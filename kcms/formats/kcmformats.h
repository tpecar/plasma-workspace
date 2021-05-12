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
#include "localelistmodel.h"
class KCMFormats : public KQuickAddons::ManagedConfigModule
{
    Q_OBJECT
    Q_PROPERTY(LocaleListModel *localeModel READ localeModel NOTIFY localeModelChanged)
    Q_PROPERTY(bool detailed READ detailed WRITE setDetailed NOTIFY detailedChanged)
    Q_PROPERTY(int globalIndex READ globalIndex WRITE setGlobalIndex NOTIFY globalIndexChanged)
    Q_PROPERTY(int langIndex READ langIndex WRITE setLangIndex NOTIFY langIndexChanged)
    Q_PROPERTY(int numericIndex READ numericIndex WRITE setNumericIndex NOTIFY numericIndexChanged)
    Q_PROPERTY(int timeIndex READ timeIndex WRITE setTimeIndex NOTIFY timeIndexChanged)
    Q_PROPERTY(int collateIndex READ collateIndex WRITE setCollateIndex NOTIFY collateIndexChanged)
    Q_PROPERTY(int monetaryIndex READ monetaryIndex WRITE setMonetaryIndex NOTIFY monetaryIndexChanged)
    Q_PROPERTY(int measurementIndex READ measurementIndex WRITE setMeasurementIndex NOTIFY measurementIndexChanged)
public:
    explicit KCMFormats(QObject *parent = nullptr, const QVariantList &list = QVariantList());
    virtual ~KCMFormats() override = default;

    LocaleListModel *localeModel() const {
        return m_localeModel;
    };
    QString loadFlagIcon(const QString &flagCode);
    bool detailed() const {
        return m_detail;
    }
    void setDetailed(bool detail) {
        m_detail = detail;
    }
    int globalIndex() const {
        return m_globalIndex;
    }
    void setGlobalIndex(int index) {
        m_globalIndex = index;
    }
    int langIndex() const {
        return m_langIndex;
    }
    void setLangIndex(int index) {
        m_langIndex = index;
    }
    int numericIndex() const {
        return m_numericIndex;
    }
    void setNumericIndex(int index) {
        m_numericIndex = index;
    }
    int timeIndex() const {
        return m_timeIndex;
    }
    void setTimeIndex(int index) {
        m_timeIndex = index;
    }
    int collateIndex() const {
        return m_collateIndex;
    }
    void setCollateIndex(int index) {
        m_collateIndex = index;
    }
    int monetaryIndex() const {
        return m_monetaryIndex;
    }
    void setMonetaryIndex(int index) {
        m_monetaryIndex = index;
    }
    int measurementIndex() const {
        return m_measurementIndex;
    }
    void setMeasurementIndex(int index) {
        m_measurementIndex = index;
    }
public Q_SLOTS:
    void load() override;
    void save() override;
    void defaults() override;
Q_SIGNALS:
    void localeModelChanged();
    void detailedChanged();
    void globalIndexChanged();
    void langIndexChanged();
    void numericIndexChanged();
    void timeIndexChanged();
    void collateIndexChanged();
    void monetaryIndexChanged();
    void measurementIndexChanged();
private:
    QHash<QString, QString> m_cachedFlags;

    void readConfig();
    void writeConfig();
    void update();

    KConfigGroup m_config;
    LocaleListModel *m_localeModel = nullptr;

    bool m_detail = false;
    int m_globalIndex = -1, m_langIndex = -1, m_numericIndex = -1, m_timeIndex = -1,
        m_collateIndex = -1, m_monetaryIndex = -1, m_measurementIndex = -1;
};

#endif
