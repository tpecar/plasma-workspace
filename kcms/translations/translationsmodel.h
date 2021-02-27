/*
 *  SPDX-FileCopyrightText: 2014 John Layt <john@layt.net>
 *  SPDX-FileCopyrightText: 2018 Eike Hein <hein@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-or-later
 */

#ifndef TRANSLATIONSMODEL_H
#define TRANSLATIONSMODEL_H

#include <QAbstractListModel>

#include <KConfigGroup>

#include <QSet>

class TranslationsModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum AdditionalRoles {
        LanguageCode = Qt::UserRole + 1,
        IsMissing,
    };
    Q_ENUM(AdditionalRoles)

    explicit TranslationsModel(QObject *parent = nullptr);
    ~TranslationsModel() override;

    QHash<int, QByteArray> roleNames() const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

protected:
    QString languageCodeToName(const QString &languageCode) const;

    static QStringList m_languages;

    static QSet<QString> m_installedLanguages;
};

class SelectedTranslationsModel : public TranslationsModel
{
    Q_OBJECT

    Q_PROPERTY(QStringList selectedLanguages READ selectedLanguages WRITE setSelectedLanguages NOTIFY selectedLanguagesChanged)
    Q_PROPERTY(QStringList missingLanguages READ missingLanguages NOTIFY missingLanguagesChanged)

public:
    explicit SelectedTranslationsModel(QObject *parent = nullptr);
    ~SelectedTranslationsModel() override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QStringList selectedLanguages() const;
    void setSelectedLanguages(const QStringList &languages);

    QStringList missingLanguages() const;

    Q_INVOKABLE void move(int from, int to);
    Q_INVOKABLE void remove(const QString &languageCode);

Q_SIGNALS:
    void selectedLanguagesChanged(const QStringList &languages) const;
    void missingLanguagesChanged() const;

private:
    QStringList m_selectedLanguages;
    QStringList m_missingLanguages;
};

class AvailableTranslationsModel : public TranslationsModel
{
    Q_OBJECT

public:
    explicit AvailableTranslationsModel(QObject *parent = nullptr);
    ~AvailableTranslationsModel() override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    Q_INVOKABLE QString langCodeAt(int row);

public Q_SLOTS:
    void setSelectedLanguages(const QStringList &languages);

private:
    QStringList m_availableLanguages;
};

#endif
