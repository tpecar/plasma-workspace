/*
 *  kcmformats.h
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
#ifndef __kcmformats_h__
#define __kcmformats_h__

#include <KQuickAddons/ManagedConfigModule>
#include <KConfigGroup>
#include <KLocalizedString>
#include "localelistmodel.h"
class FormatsSettings;
class KCMFormats : public KQuickAddons::ManagedConfigModule
{
    Q_OBJECT
    Q_PROPERTY(LocaleListModel *localeModel READ localeModel NOTIFY localeModelChanged)
    Q_PROPERTY(bool detailed READ detailed WRITE setDetailed NOTIFY detailedChanged)
    Q_PROPERTY(int langIndex READ langIndex WRITE setLangIndex NOTIFY langIndexChanged)
    Q_PROPERTY(int numericIndex READ numericIndex WRITE setNumericIndex NOTIFY numericIndexChanged)
    Q_PROPERTY(int timeIndex READ timeIndex WRITE setTimeIndex NOTIFY timeIndexChanged)
    Q_PROPERTY(int collateIndex READ collateIndex WRITE setCollateIndex NOTIFY collateIndexChanged)
    Q_PROPERTY(int monetaryIndex READ monetaryIndex WRITE setMonetaryIndex NOTIFY monetaryIndexChanged)
    Q_PROPERTY(int measurementIndex READ measurementIndex WRITE setMeasurementIndex NOTIFY measurementIndexChanged)
    Q_PROPERTY(QString numberExample READ numberExample NOTIFY numericExampleChanged)
    Q_PROPERTY(QString timeExample READ timeExample NOTIFY timeExampleChanged)
    Q_PROPERTY(QString currencyExample READ currencyExample NOTIFY monetaryExampleChanged)
    Q_PROPERTY(QString measurementExample READ measurementExample NOTIFY measurementExampleChanged)
public:
    explicit KCMFormats(QObject *parent = nullptr, const QVariantList &list = QVariantList());
    virtual ~KCMFormats() override = default;

    LocaleListModel *localeModel() const {
        return m_localeModel;
    };
    QString loadFlagIcon(const QString &flagCode);
    bool detailed() const;
    void setDetailed(bool detailed);
    int langIndex() const;
    void setLangIndex(int index);
    int numericIndex() const;
    void setNumericIndex(int index);
    int timeIndex() const;
    void setTimeIndex(int index);
    int collateIndex() const;
    void setCollateIndex(int index);
    int monetaryIndex() const;
    void setMonetaryIndex(int index);
    int measurementIndex() const;
    void setMeasurementIndex(int index);
    QString numberExample() const;
    QString timeExample() const;
    QString currencyExample() const;
    QString measurementExample() const;
    Q_INVOKABLE FormatsSettings *settings() const;
public Q_SLOTS:
    void load() override;
    void save() override;
    void defaults() override;
Q_SIGNALS:
    void detailedChanged();
    void localeModelChanged();
    void langIndexChanged();
    void numericIndexChanged();
    void timeIndexChanged();
    void collateIndexChanged();
    void monetaryIndexChanged();
    void measurementIndexChanged();
    void numericExampleChanged();
    void timeExampleChanged();
    void collateExampleChanged();
    void monetaryExampleChanged();
    void measurementExampleChanged();
    void saveClicked();

private:
    QHash<QString, QString> m_cachedFlags;

    void readConfig();
    void writeConfig();
    void update();

    FormatsSettings *m_settings = nullptr;
    LocaleListModel *m_localeModel = nullptr;
    int m_langIndex = -1, m_numericIndex = -1, m_timeIndex = -1,
        m_collateIndex = -1, m_monetaryIndex = -1, m_measurementIndex = -1;
};

#endif
