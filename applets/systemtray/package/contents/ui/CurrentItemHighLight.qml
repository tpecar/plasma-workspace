/*
 *   SPDX-FileCopyrightText: 2011 Marco Martin <mart@kde.org>
 *
 *   SPDX-License-Identifier: LGPL-2.0-or-later
 */

import QtQuick 2.12
import org.kde.plasma.plasmoid 2.0
import org.kde.plasma.core 2.0 as PlasmaCore

PlasmaCore.FrameSvgItem {
    id: expandedItem

    property int location
    property bool animationEnabled: true

    z: -1 // always draw behind icons
    width: parent.width
    height: parent.height
    opacity: parent && systemTrayState.expanded ? 1 : 0

    function changeHighlightedItem(nextItem) {
        parent = nextItem;
    }

    function changeHighlightedItemNoAnimation(nextItem) {
        animationEnabled = false;
        parent = nextItem;
        animationEnabled = true;
    }

    imagePath: "widgets/tabbar"
    prefix: {
        var prefix = ""
        switch (location) {
            case PlasmaCore.Types.LeftEdge:
                prefix = "west-active-tab";
                break;
            case PlasmaCore.Types.TopEdge:
                prefix = "north-active-tab";
                break;
            case PlasmaCore.Types.RightEdge:
                prefix = "east-active-tab";
                break;
            default:
                prefix = "south-active-tab";
            }
            if (!hasElementPrefix(prefix)) {
                prefix = "active-tab";
            }
            return prefix;
    }

    Connections {
        target: systemTrayState

        function onActiveAppletChanged() {
            if (systemTrayState.activeApplet && systemTrayState.activeApplet.parent.inVisibleLayout) {
                changeHighlightedItem(systemTrayState.activeApplet.parent.container)
            } else if (systemTrayState.expanded) {
                changeHighlightedItem(root)
            }
        }

        function onExpandedChanged() {
            if (systemTrayState.expanded && !systemTrayState.activeApplet) {
                changeHighlightedItemNoAnimation(root)
            }
        }
    }

    Behavior on opacity {
        NumberAnimation {
            duration: units.longDuration
            easing.type: parent && systemTrayState.expanded ? Easing.OutCubic : Easing.InCubic
        }
    }
    Behavior on x {
        id: xAnim
        enabled: parent && animationEnabled
        NumberAnimation {
            duration: units.longDuration
            easing.type: Easing.InOutCubic
        }
    }
    Behavior on y {
        id: yAnim
        enabled: parent && animationEnabled
        NumberAnimation {
            duration: units.longDuration
            easing.type: Easing.InOutCubic
        }
    }
    Behavior on width {
        id: widthAnim
        enabled: parent && animationEnabled
        NumberAnimation {
            duration: units.longDuration
            easing.type: Easing.InOutCubic
        }
    }
    Behavior on height {
        id: heightAnim
        enabled: parent && animationEnabled
        NumberAnimation {
            duration: units.longDuration
            easing.type: Easing.InOutCubic
        }
    }
}
