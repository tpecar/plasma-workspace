/*
 *   Copyright 2021 Kai Uwe Broulik <kde@broulik.de>
 *   Copyright 2021 David Redondo <kde@david-redondo.de>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2 or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

import QtQuick 2.0
import QtQuick.Layouts 1.1

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 3.0 as PlasmaComponents3
import QtQuick.Controls 2.0 as QQC2

RowLayout {
    id: profileItem

    property string activeProfile
    property var profiles: []
    property string inhibitionReason

    signal activateProfileRequested(string profile)

    spacing: PlasmaCore.Units.gridUnit

    PlasmaCore.IconItem {
        source: "speedometer"
        Layout.alignment: Qt.AlignTop
        Layout.preferredWidth: PlasmaCore.Units.iconSizes.medium
        Layout.preferredHeight: width
    }
    Column {
        id: profileColumn
        Layout.fillWidth: true
        Layout.alignment: Qt.AlignTop
        spacing: 0

        PlasmaComponents3.Label {
            text: i18n("Power Profile")
        }

        PlasmaComponents3.Slider {
            id: profileSlider
            width: parent.width

            readonly property var profileData:  [{
                label: i18n("Battery Life"),
                profile: "power-saver",
                inhibited: false
            }, {
                label: i18n("Balanced"),
                profile: "balanced",
                inhibited: false
            }, {
                label: i18n("Performance"),
                profile: "performance",
                inhibited: profileItem.inhibitionReason != ""
            }]

            from: 0
            to: 2
            stepSize: 1
            value: profileData.findIndex((profile => profile.profile == profileItem.activeProfile))
            snapMode: PlasmaComponents3.Slider.SnapAlways
            onMoved: {
                if (!profileData[value].inhibited) {
                    activateProfileRequested(profileData[value].profile)
                }
                // reset back, we will update when the active profile changes
                value = Qt.binding(() => profileData.findIndex((profile => profile.profile == profileItem.activeProfile)))
            }
            // fake having a disabled second half
            Rectangle {
                z: -1
                visible: profileItem.inhibitionReason != ""
                color: PlasmaCore.Theme.backgroundColor
                anchors.left: parent.horizontalCenter
                anchors.leftMargin: 1
                anchors.right: parent.right
                anchors.top: parent.background.top
                height: parent.background.height
                opacity: 0.4
            }
        }
        RowLayout {
            width: parent.width
            spacing: 0
            Repeater {
                model: profileSlider.profileData
                PlasmaComponents3.Label {
                    horizontalAlignment: index == 0 ? Text.AlignLeft : (index == profileSlider.profileData.length - 1 ? Text.AlignRight : Text.AlignHCenter)
                    Layout.fillWidth: true
                    Layout.preferredWidth: 50   // Common width for better alignment

                    text: modelData.label
                }
            }
        }

        InhibitionHint {
            visible: inhibitionReason
            width: parent.width
            iconSource: "dialog-information"
            text: inhibitionReason == "lap-detected" ? i18n("Performance mode is unavailable because the computer has detected it is sitting on your lap.") :
                  inhibitionReason == "high-operating-temperature" ? i18n("Performance mode is unavailable because the computer is running too hot.") :
                  i18n("Performance may be reduced.")
        }
    }
}
