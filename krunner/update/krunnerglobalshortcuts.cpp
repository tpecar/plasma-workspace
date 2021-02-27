/*
 *   SPDX-FileCopyrightText: 2019 Kai Uwe Broulik <kde@broulik.de>
 *   SPDX-FileCopyrightText: 2020 David Redondo <kde@david-redondo.de>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#include <QCoreApplication>
#include <QDebug>
#include <QStandardPaths>

#include <KActionCollection>
#include <KConfig>
#include <KConfigGroup>
#include <KDesktopFile>
#include <KGlobalAccel>
#include <KSharedConfig>

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    const QString oldCompomentName = QStringLiteral("krunner");
    const QString oldDesktopFile = QStringLiteral("krunner.desktop");
    const QString newDesktopFile = QStringLiteral("org.kde.krunner.desktop");

    // Since we need to fake those actions, read the translated names from the desktop file
    KDesktopFile df(QStandardPaths::GenericDataLocation, QStringLiteral("kglobalaccel/") + newDesktopFile);
    QString displayName = QStringLiteral("KRunner");
    if (!df.readName().isEmpty()) {
        displayName = df.readName();
    }
    const QString clipboardActionName = df.actionGroup(QStringLiteral("RunClipboard")).readEntry(QStringLiteral("Name"), QStringLiteral("Run command on clipboard contents"));

    KActionCollection shortCutActions(nullptr, oldDesktopFile);
    shortCutActions.setComponentDisplayName(displayName);
    QAction runCommandAction(displayName);
    shortCutActions.addAction(QStringLiteral("_launch"), &runCommandAction);
    QAction runClipboardAction(clipboardActionName);
    shortCutActions.addAction(QStringLiteral("RunClipboard"), &runClipboardAction);

    QList<QKeySequence> oldRunCommand;
    QList<QKeySequence> oldRunClipboard;
     if (KGlobalAccel::isComponentActive(oldCompomentName)) {
        oldRunCommand = KGlobalAccel::self()->globalShortcut(oldCompomentName, QStringLiteral("run command"));
        oldRunClipboard = KGlobalAccel::self()->globalShortcut(oldCompomentName, QStringLiteral("run command on clipboard contents"));
        KGlobalAccel::self()->cleanComponent(oldCompomentName);
    } else if(KGlobalAccel::isComponentActive(oldDesktopFile)) {
        oldRunCommand = KGlobalAccel::self()->globalShortcut(oldDesktopFile, runCommandAction.objectName());
        oldRunClipboard = KGlobalAccel::self()->globalShortcut(oldDesktopFile, runClipboardAction.objectName());
        KGlobalAccel::self()->cleanComponent(oldDesktopFile);
    } else {
        return 0;
    }

    shortCutActions.takeAction(&runCommandAction);
    shortCutActions.takeAction(&runClipboardAction);
    shortCutActions.setComponentName(newDesktopFile);
    shortCutActions.addActions({&runCommandAction, &runClipboardAction});

    if (!oldRunCommand.isEmpty()) {
        KGlobalAccel::self()->setShortcut(&runCommandAction, oldRunCommand, KGlobalAccel::NoAutoloading);
    }
    if (!oldRunClipboard.isEmpty()) {
        KGlobalAccel::self()->setShortcut(&runClipboardAction, oldRunClipboard, KGlobalAccel::NoAutoloading);
    }

    return 0;
}
