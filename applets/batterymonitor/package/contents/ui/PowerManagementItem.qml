/*
 *   SPDX-FileCopyrightText: 2012-2013 Daniel Nicoletti <dantti12@gmail.com>
 *   SPDX-FileCopyrightText: 2013, 2015 Kai Uwe Broulik <kde@privat.broulik.de>
 *
 *   SPDX-License-Identifier: LGPL-2.0-or-later
 */

import QtQuick 2.0
import QtQuick.Layouts 1.1
import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 3.0 as PlasmaComponents3
import org.kde.kquickcontrolsaddons 2.0

ColumnLayout {
    id: powerManagementHeadingColumn
    property alias enabled: pmCheckBox.checked
    property bool pluggedIn

    spacing: 0

    RowLayout {
        Layout.fillWidth: true
        Layout.leftMargin: units.smallSpacing

        PlasmaComponents3.CheckBox {
            id: pmCheckBox
            Layout.fillWidth: true
            text: i18nc("Minimize the length of this string as much as possible", "Allow automatic sleep and screen locking")
            checked: true
        }
    }

    ColumnLayout {
        Layout.fillWidth: true
        Layout.leftMargin: units.gridUnit + units.smallSpacing // width of checkbox and spacer
        spacing: units.smallSpacing

        InhibitionHint {
            Layout.fillWidth: true
            readonly property var chargeStopThreshold: pmSource.data["Battery"] ? pmSource.data["Battery"]["Charge Stop Threshold"] : undefined
            visible: powerManagementHeadingColumn.pluggedIn && typeof chargeStopThreshold === "number" && chargeStopThreshold > 0 && chargeStopThreshold < 100
            iconSource: "kt-speed-limits" // FIXME good icon
            text: i18n("Your battery is configured to only charge up to %1%.", chargeStopThreshold || 0)
        }

        InhibitionHint {
            Layout.fillWidth: true
            visible: pmSource.data["PowerDevil"] && pmSource.data["PowerDevil"]["Is Lid Present"] && !pmSource.data["PowerDevil"]["Triggers Lid Action"] ? true : false
            iconSource: "computer-laptop"
            text: i18nc("Minimize the length of this string as much as possible", "Your notebook is configured not to sleep when closing the lid while an external monitor is connected.")
        }

        PlasmaComponents3.Label {
            id: inhibitionExplanation
            Layout.fillWidth: true
            // Don't need to show the inhibitions when power management
            // isn't enabled anyway
            visible: inhibitions.length > 0 && pmCheckBox.checked
            font: theme.smallestFont
            wrapMode: Text.WordWrap
            elide: Text.ElideRight
            maximumLineCount: 3
            text: {
                if (inhibitions.length === 1) {
                    return i18n("An application is preventing sleep and screen locking:")
                } else if (inhibitions.length > 1) {
                    return i18np("%1 application is preventing sleep and screen locking:",
                                 "%1 applications are preventing sleep and screen locking:",
                                 inhibitions.length)
                } else {
                    return ""
                }
            }
        }
        Repeater {
            model: inhibitionExplanation.visible ? inhibitions.length : null

            InhibitionHint {
                Layout.fillWidth: true
                iconSource: inhibitions[index].Icon || ""
                text: inhibitions[index].Reason ?
                                                i18nc("Application name: reason for preventing sleep and screen locking", "%1: %2", inhibitions[index].Name, inhibitions[index].Reason)
                                                : i18nc("Application name: reason for preventing sleep and screen locking", "%1: unknown reason", inhibitions[index].Name)
            }
        }
    }
}

