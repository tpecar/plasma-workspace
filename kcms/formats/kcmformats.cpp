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
#include "formatssettings.h"
#include "kcmformats.h"
#include <KAboutData>
#include <KLocalizedString>
#include <KPluginFactory>
#include <KSharedConfig>

K_PLUGIN_CLASS_WITH_JSON(KCMFormats, "metadata.json")

KCMFormats::KCMFormats(QObject *parent, const QVariantList &args)
    : KQuickAddons::ManagedConfigModule(parent, args)
    , m_settings(new FormatsSettings(this))
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
    if (!detailed()) {
        m_numericIndex = index;
        m_timeIndex = index;
        m_monetaryIndex = index;
        m_collateIndex = index;
        m_measurementIndex = index;
        update();
    } else
        Q_EMIT langIndexChanged();

    m_settings->setLANG(m_localeModel->localeAt(m_langIndex).name());
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
    static auto readWithDefault = [](const QString &lhs, const QString &rhs) {
        if (lhs.isEmpty())
            return QString::fromUtf8(qgetenv(rhs.toLatin1().data()));
        else
            return lhs;
    };
    QLocale lang_locale(readWithDefault(m_settings->lANG(), lcLang));
    QLocale numeric_locale(readWithDefault(m_settings->lC_NUMERIC(), lcNumeric));
    QLocale time_locale(readWithDefault(m_settings->lC_TIME(), lcTime));
    QLocale collate_locale(readWithDefault(m_settings->lC_COLLATE(), lcCollate));
    QLocale monetary_locale(readWithDefault(m_settings->lC_MONETARY(), lcMonetary));
    QLocale measurement_locale(readWithDefault(m_settings->lC_MEASUREMENT(), lcMeasurement));

    setLangIndex(m_localeModel->findIndexFromLocale(lang_locale));
    m_numericIndex = m_localeModel->findIndexFromLocale(numeric_locale);
    m_timeIndex = m_localeModel->findIndexFromLocale(time_locale);
    m_collateIndex = m_localeModel->findIndexFromLocale(collate_locale);
    m_monetaryIndex = m_localeModel->findIndexFromLocale(monetary_locale);
    m_measurementIndex = m_localeModel->findIndexFromLocale(measurement_locale);

    update();
}

void KCMFormats::save()
{
    Q_EMIT saveClicked();
    const QString global = m_localeModel->localeAt(m_langIndex).name();

    m_settings->setLANG(global);
    if (!m_settings->useDetailed()) {
        // Global setting, clean up config
        m_settings->setLC_NUMERIC({});
        m_settings->setLC_TIME({});
        m_settings->setLC_MONETARY({});
        m_settings->setLC_MEASUREMENT({});
        m_settings->setLC_COLLATE({});
        m_settings->setLC_CTYPE({});
    } else {
        // Save detailed settings

        const QString numeric = m_localeModel->localeAt(m_numericIndex).name();
        m_settings->setLC_NUMERIC(numeric);

        const QString time = m_localeModel->localeAt(m_timeIndex).name();
        m_settings->setLC_TIME(time);

        const QString monetary = m_localeModel->localeAt(m_monetaryIndex).name();
        m_settings->setLC_MONETARY(monetary);

        const QString measurement = m_localeModel->localeAt(m_measurementIndex).name();
        m_settings->setLC_MEASUREMENT(measurement);

        const QString collate = m_localeModel->localeAt(m_collateIndex).name();
        m_settings->setLC_COLLATE(collate);
    }

    KQuickAddons::ManagedConfigModule::save();
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
bool KCMFormats::detailed() const
{
    return m_settings->useDetailed();
}
void KCMFormats::setDetailed(bool detailed)
{
    m_settings->setUseDetailed(detailed);
}
int KCMFormats::langIndex() const
{
    return m_langIndex;
}
int KCMFormats::numericIndex() const
{
    return m_numericIndex;
}
void KCMFormats::setNumericIndex(int index)
{
    m_numericIndex = index;
    Q_EMIT numericExampleChanged();
}
int KCMFormats::timeIndex() const
{
    return m_timeIndex;
}
void KCMFormats::setTimeIndex(int index)
{
    m_timeIndex = index;
    Q_EMIT timeExampleChanged();
}
int KCMFormats::collateIndex() const
{
    return m_collateIndex;
}
void KCMFormats::setCollateIndex(int index)
{
    m_collateIndex = index;
    Q_EMIT collateExampleChanged();
}
int KCMFormats::monetaryIndex() const
{
    return m_monetaryIndex;
}
void KCMFormats::setMonetaryIndex(int index)
{
    m_monetaryIndex = index;
    Q_EMIT monetaryExampleChanged();
}
int KCMFormats::measurementIndex() const
{
    return m_measurementIndex;
}
void KCMFormats::setMeasurementIndex(int index)
{
    m_measurementIndex = index;
    Q_EMIT measurementExampleChanged();
}
QString KCMFormats::numberExample() const
{
    return m_localeModel->localeAt(m_numericIndex).toString(1000.01);
}
QString KCMFormats::timeExample() const
{
    auto tloc = m_localeModel->localeAt(m_timeIndex);
    return i18n("%1 (long format)", tloc.toString(QDateTime::currentDateTime())) + QLatin1Char('\n')
        + i18n("%1 (short format)", tloc.toString(QDateTime::currentDateTime(), QLocale::ShortFormat));
}
QString KCMFormats::currencyExample() const
{
    return m_localeModel->localeAt(m_monetaryIndex).toCurrencyString(24.00);
}
QString KCMFormats::measurementExample() const
{
    auto mloc = m_localeModel->localeAt(m_measurementIndex);
    QString measurementExample;
    if (mloc.measurementSystem() == QLocale::ImperialUKSystem) {
        measurementExample = i18nc("Measurement combobox", "Imperial UK");
    } else if (mloc.measurementSystem() == QLocale::ImperialUSSystem || mloc.measurementSystem() == QLocale::ImperialSystem) {
        measurementExample = i18nc("Measurement combobox", "Imperial US");
    } else {
        measurementExample = i18nc("Measurement combobox", "Metric");
    }
    return measurementExample;
}
FormatsSettings *KCMFormats::settings() const
{
    return m_settings;
}
#include "kcmformats.moc"
