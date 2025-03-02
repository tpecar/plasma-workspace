/***************************************************************************
 *   Copyright (C) 2020 by Méven Car <meven.car@enioka.com>                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA          *
 ***************************************************************************/
#include "autostartmodel.h"

#include <KConfigGroup>
#include <KDesktopFile>
#include <KShell>
#include <QDebug>
#include <QDir>
#include <QQuickItem>
#include <QQuickRenderControl>
#include <QStandardPaths>
#include <QWindow>

#include <KFileItem>
#include <KIO/CopyJob>
#include <KIO/DeleteJob>
#include <KLocalizedString>
#include <KOpenWithDialog>
#include <KPropertiesDialog>
#include <autostartscriptdesktopfile.h>

// FDO user autostart directories are
// .config/autostart which has .desktop files executed by klaunch or systemd, some of which might be scripts

// Then we have Plasma-specific locations which run scripts
// .config/autostart-scripts which has scripts executed by plasma_session (now migrated to .desktop files)
// .config/plasma-workspace/shutdown which has scripts executed by plasma-shutdown
// .config/plasma-workspace/env which has scripts executed by startplasma

// in the case of pre-startup they have to end in .sh
// everywhere else it doesn't matter

// the comment above describes how autostart *currently* works, it is not definitive documentation on how autostart *should* work

// share/autostart shouldn't be an option as this should be reserved for global autostart entries

std::optional<AutostartEntry> AutostartModel::loadDesktopEntry(const QString &fileName)
{
    KDesktopFile config(fileName);
    const KConfigGroup grp = config.desktopGroup();
    const auto name = config.readName();

    const bool hidden = grp.readEntry("Hidden", false);

    if (hidden) {
        return {};
    }

    const QStringList notShowList = grp.readXdgListEntry("NotShowIn");
    const QStringList onlyShowList = grp.readXdgListEntry("OnlyShowIn");
    const bool enabled = !(notShowList.contains(QLatin1String("KDE")) || (!onlyShowList.isEmpty() && !onlyShowList.contains(QLatin1String("KDE"))));

    if (!enabled) {
        return {};
    }

    const auto lstEntry = grp.readXdgListEntry("OnlyShowIn");
    const bool onlyInPlasma = lstEntry.contains(QLatin1String("KDE"));
    const QString iconName = config.readIcon();
    const auto kind = AutostartScriptDesktopFile::isAutostartScript(config) ? XdgScripts : XdgAutoStart; // .config/autostart load desktop at startup
    const QString tryCommand = grp.readEntry("TryExec");

    // Try to filter out entries that point to nonexistant programs
    // If TryExec is either found in $PATH or is an absolute file path that exists
    // This doesn't detect uninstalled Flatpaks for example though
    if (!tryCommand.isEmpty() && QStandardPaths::findExecutable(tryCommand).isEmpty() && !QFile::exists(tryCommand)) {
        return {};
    }

    return AutostartEntry{name, kind, enabled, fileName, onlyInPlasma, iconName};
}

AutostartModel::AutostartModel(QObject *parent)
    : QAbstractListModel(parent)
    , m_xdgConfigPath(QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation))
    , m_xdgAutoStartPath(m_xdgConfigPath.filePath(QStringLiteral("autostart")))
{
}

void AutostartModel::load()
{
    beginResetModel();

    m_entries.clear();

    // Creates if doesn't already exist
    m_xdgAutoStartPath.mkpath(QStringLiteral("."));

    // Needed to add all script entries after application entries
    QVector<AutostartEntry> scriptEntries;
    const auto filesInfo = m_xdgAutoStartPath.entryInfoList(QDir::Files);
    for (const QFileInfo &fi : filesInfo) {
        if (!KDesktopFile::isDesktopFile(fi.fileName())) {
            continue;
        }

        const std::optional<AutostartEntry> entry = loadDesktopEntry(fi.absoluteFilePath());

        if (!entry) {
            continue;
        }

        if (entry->source == XdgScripts) {
            scriptEntries.push_back(entry.value());
        } else {
            m_entries.push_back(entry.value());
        }
    }

    m_entries.append(scriptEntries);

    loadScriptsFromDir(QStringLiteral("plasma-workspace/env/"), AutostartModel::AutostartEntrySource::PlasmaEnvScripts);

    loadScriptsFromDir(QStringLiteral("plasma-workspace/shutdown/"), AutostartModel::AutostartEntrySource::PlasmaShutdown);

    endResetModel();
}

void AutostartModel::loadScriptsFromDir(const QString &subDir, AutostartModel::AutostartEntrySource kind)
{
    QDir dir(m_xdgConfigPath.filePath(subDir));
    // Creates if doesn't already exist
    dir.mkpath(QStringLiteral("."));

    const auto autostartDirFilesInfo = dir.entryInfoList(QDir::Files);
    for (const QFileInfo &fi : autostartDirFilesInfo) {
        QString fileName = fi.absoluteFilePath();
        const bool isSymlink = fi.isSymLink();
        if (isSymlink) {
            fileName = fi.symLinkTarget();
        }

        m_entries.push_back({fileName, kind, true, fi.absoluteFilePath(), false, QStringLiteral("dialog-scripts")});
    }
}

int AutostartModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }

    return m_entries.count();
}

bool AutostartModel::reloadEntry(const QModelIndex &index, const QString &fileName)
{
    if (!checkIndex(index)) {
        return false;
    }

    const std::optional<AutostartEntry> newEntry = loadDesktopEntry(fileName);

    if (!newEntry) {
        return false;
    }

    m_entries.replace(index.row(), newEntry.value());
    Q_EMIT dataChanged(index, index);
    return true;
}

QVariant AutostartModel::data(const QModelIndex &index, int role) const
{
    if (!checkIndex(index)) {
        return QVariant();
    }

    const auto &entry = m_entries.at(index.row());

    switch (role) {
    case Qt::DisplayRole:
        return entry.name;
    case Enabled:
        return entry.enabled;
    case Source:
        return entry.source;
    case FileName:
        return entry.fileName;
    case OnlyInPlasma:
        return entry.onlyInPlasma;
    case IconName:
        return entry.iconName;
    }

    return QVariant();
}

void AutostartModel::addApplication(const KService::Ptr &service)
{
    QString desktopPath;
    // It is important to ensure that we make an exact copy of an existing
    // desktop file (if selected) to enable users to override global autostarts.
    // Also see
    // https://bugs.launchpad.net/ubuntu/+source/kde-workspace/+bug/923360
    if (service->desktopEntryName().isEmpty() || service->entryPath().isEmpty()) {
        // create a new desktop file in s_desktopPath
        desktopPath = m_xdgAutoStartPath.filePath(service->name() + QStringLiteral(".desktop"));

        KDesktopFile desktopFile(desktopPath);
        KConfigGroup kcg = desktopFile.desktopGroup();
        kcg.writeEntry("Name", service->name());
        kcg.writeEntry("Exec", service->exec());
        kcg.writeEntry("Icon", service->icon());
        kcg.writeEntry("Path", "");
        kcg.writeEntry("Terminal", service->terminal() ? "True" : "False");
        kcg.writeEntry("Type", "Application");
        desktopFile.sync();

    } else {
        desktopPath = m_xdgAutoStartPath.filePath(service->desktopEntryName() + QStringLiteral(".desktop"));

        QFile::remove(desktopPath);

        // copy original desktop file to new path
        KDesktopFile desktopFile(service->entryPath());
        auto newDeskTopFile = desktopFile.copyTo(desktopPath);
        newDeskTopFile->sync();
    }

    const auto entry = AutostartEntry{service->name(),
                                      AutostartModel::AutostartEntrySource::XdgAutoStart, // .config/autostart load desktop at startup
                                      true,
                                      desktopPath,
                                      false,
                                      service->icon()};

    int lastApplication = -1;
    for (const AutostartEntry &e : qAsConst(m_entries)) {
        if (e.source == AutostartModel::AutostartEntrySource::XdgScripts) {
            break;
        }
        ++lastApplication;
    }

    // push before the script items
    const int index = lastApplication + 1;

    beginInsertRows(QModelIndex(), index, index);

    m_entries.insert(index, entry);

    endInsertRows();
}

void AutostartModel::showApplicationDialog(QQuickItem *context)
{
    KOpenWithDialog *owdlg = new KOpenWithDialog();
    owdlg->setAttribute(Qt::WA_DeleteOnClose);

    if (context && context->window()) {
        if (QWindow *actualWindow = QQuickRenderControl::renderWindowFor(context->window())) {
            owdlg->winId(); // so it creates windowHandle
            owdlg->windowHandle()->setTransientParent(actualWindow);
            owdlg->setModal(true);
        }
    }

    connect(owdlg, &QDialog::finished, this, [this, owdlg](int result) {
        if (result != QDialog::Accepted) {
            return;
        }

        const KService::Ptr service = owdlg->service();

        Q_ASSERT(service);
        if (!service) {
            return; // Don't crash if KOpenWith wasn't able to create service.
        }

        addApplication(service);
    });
    owdlg->open();
}

void AutostartModel::addScript(const QUrl &url, AutostartModel::AutostartEntrySource kind)
{
    const QFileInfo file(url.toLocalFile());

    if (!file.isAbsolute()) {
        Q_EMIT error(i18n("\"%1\" is not an absolute url.", url.toLocalFile()));
        return;
    } else if (!file.exists()) {
        Q_EMIT error(i18n("\"%1\" does not exist.", url.toLocalFile()));
        return;
    } else if (!file.isFile()) {
        Q_EMIT error(i18n("\"%1\" is not a file.", url.toLocalFile()));
        return;
    } else if (!file.isReadable()) {
        Q_EMIT error(i18n("\"%1\" is not readable.", url.toLocalFile()));
        return;
    }

    const QString fileName = url.fileName();

    if (kind == AutostartModel::AutostartEntrySource::XdgScripts) {
        int lastLoginScript = -1;
        for (const AutostartEntry &e : qAsConst(m_entries)) {
            if (e.source == AutostartModel::AutostartEntrySource::PlasmaShutdown) {
                break;
            }
            ++lastLoginScript;
        }

        AutostartScriptDesktopFile desktopFile(fileName, file.filePath());
        insertScriptEntry(lastLoginScript + 1, fileName, desktopFile.fileName(), kind);
    } else if (kind == AutostartModel::AutostartEntrySource::PlasmaShutdown) {
        const QUrl destinationScript = QUrl::fromLocalFile(QDir(m_xdgConfigPath.filePath(QStringLiteral("plasma-workspace/shutdown/"))).filePath(fileName));
        KIO::CopyJob *job = KIO::link(url, destinationScript, KIO::HideProgressInfo);
        job->setAutoRename(true);
        job->setProperty("finalUrl", destinationScript);

        connect(job, &KIO::CopyJob::renamed, this, [](KIO::Job *job, const QUrl &from, const QUrl &to) {
            Q_UNUSED(from)
            // in case the destination filename had to be renamed
            job->setProperty("finalUrl", to);
        });

        connect(job, &KJob::finished, this, [this, url, kind](KJob *theJob) {
            if (theJob->error()) {
                qWarning() << "Could not add script entry" << theJob->errorString();
                return;
            }
            const QUrl dest = theJob->property("finalUrl").toUrl();
            insertScriptEntry(m_entries.size(), dest.fileName(), dest.path(), kind);
        });

        job->start();
    } else {
        Q_ASSERT(0);
    }
}

void AutostartModel::insertScriptEntry(int index, const QString &name, const QString &path, AutostartEntrySource kind)
{
    beginInsertRows(QModelIndex(), index, index);

    AutostartEntry entry = AutostartEntry{name, kind, true, path, false, QStringLiteral("dialog-scripts")};

    m_entries.insert(index, entry);

    endInsertRows();
}

void AutostartModel::removeEntry(int row)
{
    const auto entry = m_entries.at(row);

    KIO::DeleteJob *job = KIO::del(QUrl::fromLocalFile(entry.fileName), KIO::HideProgressInfo);

    connect(job, &KJob::finished, this, [this, row, entry](KJob *theJob) {
        if (theJob->error()) {
            qWarning() << "Could not remove entry" << theJob->errorString();
            return;
        }

        beginRemoveRows(QModelIndex(), row, row);
        m_entries.remove(row);

        endRemoveRows();
    });

    job->start();
}

QHash<int, QByteArray> AutostartModel::roleNames() const
{
    QHash<int, QByteArray> roleNames = QAbstractListModel::roleNames();

    roleNames.insert(Name, QByteArrayLiteral("name"));
    roleNames.insert(Enabled, QByteArrayLiteral("enabled"));
    roleNames.insert(Source, QByteArrayLiteral("source"));
    roleNames.insert(FileName, QByteArrayLiteral("fileName"));
    roleNames.insert(OnlyInPlasma, QByteArrayLiteral("onlyInPlasma"));
    roleNames.insert(IconName, QByteArrayLiteral("iconName"));

    return roleNames;
}

void AutostartModel::editApplication(int row, QQuickItem *context)
{
    const QModelIndex idx = index(row, 0);

    const QString fileName = data(idx, AutostartModel::Roles::FileName).toString();
    KFileItem kfi(QUrl::fromLocalFile(fileName));
    kfi.setDelayedMimeTypes(true);

    KPropertiesDialog *dlg = new KPropertiesDialog(kfi, nullptr);
    dlg->setAttribute(Qt::WA_DeleteOnClose);

    if (context && context->window()) {
        if (QWindow *actualWindow = QQuickRenderControl::renderWindowFor(context->window())) {
            dlg->winId(); // so it creates windowHandle
            dlg->windowHandle()->setTransientParent(actualWindow);
            dlg->setModal(true);
        }
    }

    connect(dlg, &QDialog::finished, this, [this, idx, dlg](int result) {
        if (result == QDialog::Accepted) {
            reloadEntry(idx, dlg->item().localPath());
        }
    });
    dlg->open();
}
