/*
 *  kcmformats.cpp
 *  Copyright 2014 Sebastian Kügler <sebas@kde.org>
 *  Copyright 2021 Han Young <hanyoung@protonmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 */
#include <QtGlobal>

// own
#include "kcmformats.h"
#include <KPluginFactory>
#include <KSharedConfig>
#include <KLocalizedString>
#include <KAboutData>

K_PLUGIN_CLASS_WITH_JSON(KCMFormats, "metadata.json")

KCMFormats::KCMFormats(QObject *parent, const QVariantList &args)
    : KQuickAddons::ManagedConfigModule(parent, args)
    , m_localeModel(new LocaleListModel(this))
{
    KAboutData *aboutData = new KAboutData(QStringLiteral("kcm_formats"),
            i18nc("@title", "formats"),
            QStringLiteral("0.1"),
            QStringLiteral(""),
            KAboutLicense::LicenseKey::GPL_V2,
            i18nc("@info:credit", "Copyright Year Author"));

        aboutData->addAuthor(i18nc("@info:credit", "Author"),
                            i18nc("@info:credit", "Author"),
                            QStringLiteral("author@domain.com"));

        setAboutData(aboutData);
        setButtons(Help | Apply | Default);
}
void KCMFormats::setLangIndex(int index)
{
    if (index == m_langIndex)
        return;

    m_langIndex = index;
    if (!m_detail) {
        m_numericIndex = index;
        m_timeIndex = index;
        m_monetaryIndex = index;
        m_collateIndex = index;
        m_measurementIndex = index;
        update();
    } else
        Q_EMIT langIndexChanged();
}
bool countryLessThan(const QLocale &c1, const QLocale &c2)
{
    // Ensure that the "Default (C)" locale always appears at the top
    if (c1.name() == QLatin1Char('C') && c2.name() != QLatin1String("C"))
        return true;
    if (c2.name() == QLatin1Char('C'))
        return false;

    const QString ncn1 = !c1.nativeCountryName().isEmpty() ? c1.nativeCountryName() : QLocale::countryToString(c1.country());
    const QString ncn2 = !c2.nativeCountryName().isEmpty() ? c2.nativeCountryName() : QLocale::countryToString(c2.country());
    return QString::localeAwareCompare(ncn1, ncn2) < 0;
}

void KCMFormats::load()
{
    QList<QLocale> allLocales = QLocale::matchingLocales(QLocale::AnyLanguage, QLocale::AnyScript, QLocale::AnyCountry);
    std::sort(allLocales.begin(), allLocales.end(), countryLessThan);
    m_localeModel->init(allLocales);
    readConfig();

    //    connect(this, &KCMFormats::langIndexChanged, this, &KCMFormats::settingsChanged);
    //    connect(this, &KCMFormats::numericIndexChanged, this, &KCMFormats::settingsChanged);
    //    connect(this, &KCMFormats::detailedChanged, this, &KCMFormats::settingsChanged);
    //    connect(this, &KCMFormats::collateIndexChanged, this, &KCMFormats::settingsChanged);
    //    connect(this, &KCMFormats::monetaryIndexChanged, this, &KCMFormats::settingsChanged);
    //    connect(this, &KCMFormats::measurementIndexChanged, this, &KCMFormats::settingsChanged);
    //    connect(this, &KCMFormats::timeIndexChanged, this, &KCMFormats::settingsChanged);
}

QString KCMFormats::loadFlagIcon(const QString &flagCode)
{
    auto icon = m_cachedFlags.value(flagCode);
    if (!icon.isEmpty()) {
        return icon;
    }

    icon = QStandardPaths::locate(QStandardPaths::GenericDataLocation, QStringLiteral("kf5/locale/countries/%1/flag.png").arg(flagCode));

    if (icon.isEmpty()) {
        icon = QStringLiteral("unknown");
    }
    m_cachedFlags.insert(flagCode, icon);

    return icon;
}

const static QString configFile = QStringLiteral("plasma-localerc");
const static QString exportFile = QStringLiteral("plasma-locale-settings.sh");

const static QString lcLang = QStringLiteral("LANG");
const static QString lcNumeric = QStringLiteral("LC_NUMERIC");
const static QString lcTime = QStringLiteral("LC_TIME");
const static QString lcMonetary = QStringLiteral("LC_MONETARY");
const static QString lcMeasurement = QStringLiteral("LC_MEASUREMENT");
const static QString lcCollate = QStringLiteral("LC_COLLATE");
const static QString lcCtype = QStringLiteral("LC_CTYPE");

const static QString lcLanguage = QStringLiteral("LANGUAGE");

void KCMFormats::readConfig()
{
    m_config = KConfigGroup(KSharedConfig::openConfig(configFile), "Formats");

    m_detail = m_config.readEntry("useDetailed", false);
    Q_EMIT detailedChanged();

    QLocale lang_locale(m_config.readEntry(lcLang, QString::fromUtf8(qgetenv(lcLang.toLatin1().data()))));
    QLocale numeric_locale(m_config.readEntry(lcLang, QString::fromUtf8(qgetenv(lcNumeric.toLatin1().data()))));
    QLocale time_locale(m_config.readEntry(lcLang, QString::fromUtf8(qgetenv(lcTime.toLatin1().data()))));
    QLocale collate_locale(m_config.readEntry(lcLang, QString::fromUtf8(qgetenv(lcCollate.toLatin1().data()))));
    QLocale monetary_locale(m_config.readEntry(lcLang, QString::fromUtf8(qgetenv(lcMonetary.toLatin1().data()))));
    QLocale measurement_locale(m_config.readEntry(lcLang, QString::fromUtf8(qgetenv(lcMeasurement.toLatin1().data()))));

    m_langIndex = m_localeModel->findIndexFromLocale(lang_locale);
    m_numericIndex = m_localeModel->findIndexFromLocale(numeric_locale);
    m_timeIndex = m_localeModel->findIndexFromLocale(time_locale);
    m_collateIndex = m_localeModel->findIndexFromLocale(collate_locale);
    m_monetaryIndex = m_localeModel->findIndexFromLocale(monetary_locale);
    m_measurementIndex = m_localeModel->findIndexFromLocale(measurement_locale);

    update();
}

void KCMFormats::writeConfig()
{
    m_config = KConfigGroup(KSharedConfig::openConfig(configFile), "Formats");

    const QString global = m_localeModel->localeAt(m_langIndex).name();

    if (m_detail) {
        // Global setting, clean up config
        m_config.deleteEntry("useDetailed");
        if (global.isEmpty()) {
            m_config.deleteEntry(lcLang);
        } else {
            m_config.writeEntry(lcLang, global);
        }
        m_config.deleteEntry(lcNumeric);
        m_config.deleteEntry(lcTime);
        m_config.deleteEntry(lcMonetary);
        m_config.deleteEntry(lcMeasurement);
        m_config.deleteEntry(lcCollate);
        m_config.deleteEntry(lcCtype);
    } else {
        // Save detailed settings
        m_config.writeEntry("useDetailed", true);

        if (global.isEmpty()) {
            m_config.deleteEntry(lcLang);
        } else {
            m_config.writeEntry(lcLang, global);
        }

        const QString numeric = m_localeModel->localeAt(m_numericIndex).name();
        if (numeric.isEmpty()) {
            m_config.deleteEntry(lcNumeric);
        } else {
            m_config.writeEntry(lcNumeric, numeric);
        }

        const QString time = m_localeModel->localeAt(m_timeIndex).name();
        if (time.isEmpty()) {
            m_config.deleteEntry(lcTime);
        } else {
            m_config.writeEntry(lcTime, time);
        }

        const QString monetary = m_localeModel->localeAt(m_monetaryIndex).name();
        if (monetary.isEmpty()) {
            m_config.deleteEntry(lcMonetary);
        } else {
            m_config.writeEntry(lcMonetary, monetary);
        }

        const QString measurement = m_localeModel->localeAt(m_measurementIndex).name();
        if (measurement.isEmpty()) {
            m_config.deleteEntry(lcMeasurement);
        } else {
            m_config.writeEntry(lcMeasurement, measurement);
        }

        const QString collate = m_localeModel->localeAt(m_collateIndex).name();
        if (collate.isEmpty()) {
            m_config.deleteEntry(lcCollate);
        } else {
            m_config.writeEntry(lcCollate, collate);
        }
    }

    m_config.sync();
}

void KCMFormats::save()
{
    writeConfig();
    Q_EMIT saveClicked();
    //    KMessageBox::information(this,
    //                             i18n("Your changes will take effect the next time you log in."),
    //                             i18n("Format Settings Changed"),
    //                             QStringLiteral("FormatSettingsChanged"));
}

void KCMFormats::defaults()
{
    readConfig();
}
void KCMFormats::update()
{
    Q_EMIT langIndexChanged();
    Q_EMIT numericIndexChanged();
    Q_EMIT timeIndexChanged();
    Q_EMIT collateIndexChanged();
    Q_EMIT monetaryIndexChanged();
    Q_EMIT measurementIndexChanged();
    Q_EMIT numericExampleChanged();
    Q_EMIT timeExampleChanged();
    Q_EMIT collateExampleChanged();
    Q_EMIT monetaryExampleChanged();
    Q_EMIT measurementExampleChanged();
}
#include "kcmformats.moc"
