/*
 *   SPDX-FileCopyrightText: 2019 Marco Martin <mart@kde.org>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once

#include <Plasma/Applet>
#include <QPointer>
#include <QStandardItemModel>

#include <KDesktopFile>
#include <KPackage/Package>

class ApplicationListModel;
class QQuickItem;

namespace KSysGuard
{
class SensorFace;
class SensorFaceController;
}

class KConfigLoader;

class SystemMonitor : public Plasma::Applet
{
    Q_OBJECT

    Q_PROPERTY(KSysGuard::SensorFaceController *faceController READ faceController CONSTANT)

public:
    SystemMonitor(QObject *parent, const QVariantList &args);
    ~SystemMonitor() override;

    void init() override;

    KSysGuard::SensorFaceController *faceController() const;

public Q_SLOTS:
    void configChanged() override;

private:
    KSysGuard::SensorFaceController *m_sensorFaceController = nullptr;
    QString m_pendingStartupPreset;
};
