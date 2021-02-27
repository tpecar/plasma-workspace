/*
 * SPDX-FileCopyrightText: 2018-2019 Kai Uwe Broulik <kde@privat.broulik.de>
 *
 * SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
 */

import QtQuick 2.8
import QtQuick.Layouts 1.1

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 3.0 as PlasmaComponents3

import org.kde.quickcharts 1.0 as Charts

import "global"

MouseArea {
    id: compactRoot

    readonly property bool inPanel: (plasmoid.location === PlasmaCore.Types.TopEdge
        || plasmoid.location === PlasmaCore.Types.RightEdge
        || plasmoid.location === PlasmaCore.Types.BottomEdge
        || plasmoid.location === PlasmaCore.Types.LeftEdge)

    Layout.minimumWidth: plasmoid.formFactor === PlasmaCore.Types.Horizontal ? height : units.iconSizes.small
    Layout.minimumHeight: plasmoid.formFactor === PlasmaCore.Types.Vertical ? width : (units.iconSizes.small + 2 * theme.mSize(theme.defaultFont).height)

    Layout.maximumWidth: inPanel ? units.iconSizeHints.panel : -1
    Layout.maximumHeight: inPanel ? units.iconSizeHints.panel : -1

    acceptedButtons: Qt.LeftButton | Qt.MiddleButton

    property int activeCount: 0
    property int unreadCount: 0

    property int jobsCount: 0
    property int jobsPercentage: 0

    property bool inhibited: false

    property bool wasExpanded: false
    onPressed: wasExpanded = plasmoid.expanded
    onClicked: {
        if (mouse.button === Qt.MiddleButton) {
            Globals.toggleDoNotDisturbMode();
        } else {
            plasmoid.expanded = !wasExpanded;
        }
    }

    PlasmaCore.Svg {
        id: notificationSvg
        imagePath: "icons/notification"
        colorGroup: PlasmaCore.ColorScope.colorGroup
    }

    PlasmaCore.SvgItem {
        id: notificationIcon
        anchors.centerIn: parent
        width: units.roundToIconSize(Math.min(parent.width, parent.height))
        height: width
        svg: notificationSvg
        visible: opacity > 0

        elementId: "notification-inactive"

        Charts.PieChart {
            id: chart

            anchors.fill: parent

            visible: false

            range { from: 0; to: 100; automatic: false }

            valueSources: Charts.SingleValueSource { value: compactRoot.jobsPercentage }
            colorSource: Charts.SingleValueSource { value: theme.highlightColor }

            thickness: units.devicePixelRatio * 5
        }

        PlasmaComponents3.Label {
            id: countLabel
            anchors.centerIn: parent
            width: Math.round(Math.min(parent.width, parent.height) * (text.length > 1 ? 0.67 : 0.75))
            height: width
            fontSizeMode: Text.Fit
            font.pointSize: 1024
            font.pixelSize: -1
            minimumPointSize: 5//theme.smallestFont.pointSize
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            text: compactRoot.unreadCount || ""
            renderType: Text.QtRendering
            visible: false
        }

        PlasmaComponents3.BusyIndicator {
            id: busyIndicator
            anchors.fill: parent
            visible: false
            running: visible
        }
    }

    PlasmaCore.IconItem {
        id: dndIcon
        anchors.fill: parent
        source: "notifications-disabled"
        opacity: 0
        scale: 2
        visible: opacity > 0
    }

    states: [
        State { // active process
            when: compactRoot.jobsCount > 0
            PropertyChanges {
                target: notificationIcon
                elementId: "notification-progress-inactive"
            }
            PropertyChanges {
                target: countLabel
                text: compactRoot.jobsCount
                visible: true
            }
            PropertyChanges {
                target: busyIndicator
                visible: compactRoot.jobsPercentage == 0
            }
            PropertyChanges {
                target: jobProgressItem
                visible: true
            }
            PropertyChanges {
                target: chart
                visible: true
            }
        },
        State { // do not disturb
            when: compactRoot.inhibited
            PropertyChanges {
                target: dndIcon
                scale: 1
                opacity: 1
            }
            PropertyChanges {
                target: notificationIcon
                scale: 0
                opacity: 0
            }
        },
        State { // unread notifications
            name: "UNREAD"
            when: compactRoot.unreadCount > 0
            PropertyChanges {
                target: notificationIcon
                elementId: "notification-active"
            }
        }
    ]

    transitions: [
        Transition {
            to: "*" // any state
            NumberAnimation {
                targets: [notificationIcon, dndIcon]
                properties: "opacity,scale"
                duration: units.longDuration
                easing.type: Easing.InOutQuad
            }
        },
        Transition {
            from: ""
            to: "UNREAD"
            SequentialAnimation {
                RotationAnimation {
                    target: notificationIcon
                    to: 30
                    easing.type: Easing.InOutQuad
                    duration: units.longDuration
                }
                RotationAnimation {
                    target: notificationIcon
                    to: -30
                    easing.type: Easing.InOutQuad
                    duration: units.longDuration * 2 // twice the swing distance, keep speed uniform
                }
                RotationAnimation {
                    target: notificationIcon
                    to: 0
                    easing.type: Easing.InOutQuad
                    duration: units.longDuration
                }
            }
        }
    ]

}
