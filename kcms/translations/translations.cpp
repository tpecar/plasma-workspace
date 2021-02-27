/*
 *  SPDX-FileCopyrightText: 2014 John Layt <john@layt.net>
 *  SPDX-FileCopyrightText: 2018 Eike Hein <hein@kde.org>
 *  SPDX-FileCopyrightText: 2019 Kevin Ottens <kevin.ottens@enioka.com>
 *
 *  SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "translations.h"
#include "translationsdata.h"
#include "translationsmodel.h"
#include "translationssettings.h"

#include <KAboutData>
#include <KLocalizedString>
#include <KPluginFactory>
#include <KSharedConfig>

K_PLUGIN_FACTORY_WITH_JSON(TranslationsFactory, "kcm_translations.json", registerPlugin<Translations>(); registerPlugin<TranslationsData>();)

Translations::Translations(QObject *parent, const QVariantList &args)
    : KQuickAddons::ManagedConfigModule(parent, args)
    , m_data(new TranslationsData(this))
    , m_translationsModel(new TranslationsModel(this))
    , m_selectedTranslationsModel(new SelectedTranslationsModel(this))
    , m_availableTranslationsModel(new AvailableTranslationsModel(this))
    , m_everSaved(false)
{
    KAboutData *about =
        new KAboutData(QStringLiteral("kcm_translations"), i18n("Configure Plasma translations"), QStringLiteral("2.0"), QString(), KAboutLicense::LGPL);
    setAboutData(about);

    setButtons(Apply | Default);

    connect(m_selectedTranslationsModel, &SelectedTranslationsModel::selectedLanguagesChanged, this, &Translations::selectedLanguagesChanged);
    connect(m_selectedTranslationsModel,
            &SelectedTranslationsModel::selectedLanguagesChanged,
            m_availableTranslationsModel,
            &AvailableTranslationsModel::setSelectedLanguages);
}

Translations::~Translations()
{
}

QAbstractItemModel *Translations::translationsModel() const
{
    return m_translationsModel;
}

QAbstractItemModel *Translations::selectedTranslationsModel() const
{
    return m_selectedTranslationsModel;
}

QAbstractItemModel *Translations::availableTranslationsModel() const
{
    return m_availableTranslationsModel;
}

bool Translations::everSaved() const
{
    return m_everSaved;
}

void Translations::load()
{
    KQuickAddons::ManagedConfigModule::load();
    m_availableTranslationsModel->setSelectedLanguages(settings()->configuredLanguages());
    m_selectedTranslationsModel->setSelectedLanguages(settings()->configuredLanguages());
}

void Translations::save()
{
    m_everSaved = true;
    emit everSavedChanged();
    KQuickAddons::ManagedConfigModule::save();
}

void Translations::defaults()
{
    KQuickAddons::ManagedConfigModule::defaults();
    m_availableTranslationsModel->setSelectedLanguages(settings()->configuredLanguages());
    m_selectedTranslationsModel->setSelectedLanguages(settings()->configuredLanguages());
}

void Translations::selectedLanguagesChanged()
{
    auto configuredLanguages = m_selectedTranslationsModel->selectedLanguages();

    const auto missingLanguages = m_selectedTranslationsModel->missingLanguages();
    for (const auto &lang : missingLanguages) {
        configuredLanguages.removeOne(lang);
    }

    settings()->setConfiguredLanguages(configuredLanguages);
    m_selectedTranslationsModel->setSelectedLanguages(configuredLanguages);
}

TranslationsSettings *Translations::settings() const
{
    return m_data->settings();
}

bool Translations::isSaveNeeded() const
{
    return !m_selectedTranslationsModel->missingLanguages().isEmpty();
}

#include "translations.moc"
