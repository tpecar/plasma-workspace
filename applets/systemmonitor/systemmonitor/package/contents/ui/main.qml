/*
 *   SPDX-FileCopyrightText: 2019 Marco Martin <mart@kde.org>
 *   SPDX-FileCopyrightText: 2019 David Edmundson <davidedmundson@kde.org>
 *   SPDX-FileCopyrightText: 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 *   SPDX-License-Identifier: LGPL-2.0-or-later
 */

import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Window 2.12

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.plasmoid 2.0

import org.kde.ksysguard.sensors 1.0 as Sensors
import org.kde.ksysguard.faces 1.0 as Faces

import org.kde.quickcharts 1.0 as Charts

Item {
    Plasmoid.backgroundHints: PlasmaCore.Types.DefaultBackground | PlasmaCore.Types.ConfigurableBackground

    Plasmoid.switchWidth: Plasmoid.formFactor === PlasmaCore.Types.Planar
        ? -1
        : (Plasmoid.fullRepresentationItem ? Plasmoid.fullRepresentationItem.Layout.minimumWidth : units.gridUnit * 8)
    Plasmoid.switchHeight: Plasmoid.formFactor === PlasmaCore.Types.Planar
        ? -1
        : (Plasmoid.fullRepresentationItem ? Plasmoid.fullRepresentationItem.Layout.minimumHeight : units.gridUnit * 12)

    Plasmoid.preferredRepresentation: Plasmoid.formFactor === PlasmaCore.Types.Planar ? Plasmoid.fullRepresentation : null

    Plasmoid.title: plasmoid.nativeInterface.faceController.title || i18n("System Monitor")
    Plasmoid.toolTipSubText: ""

    Plasmoid.compactRepresentation: CompactRepresentation {
    }
    Plasmoid.fullRepresentation: FullRepresentation {
    }

    Plasmoid.configurationRequired: plasmoid.nativeInterface.faceController.highPrioritySensorIds.length == 0 && plasmoid.nativeInterface.faceController.lowPrioritySensorIds.length == 0 && plasmoid.nativeInterface.faceController.totalSensor.length == 0
}
