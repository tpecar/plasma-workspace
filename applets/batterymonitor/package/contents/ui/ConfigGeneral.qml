/*
 *  SPDX-FileCopyrightText: 2016 Marco Martin <mart@kde.org>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.0
import QtQuick.Controls 2.5
import org.kde.kirigami 2.4 as Kirigami

Item {
    id: root

    width: pageColumn.width
    height: pageColumn.height

    property alias cfg_showPercentage: showPercentage.checked

    Kirigami.FormLayout {
        id: pageColumn
        anchors.left: parent.left
        anchors.right: parent.right

        CheckBox {
            id: showPercentage
            text: i18n("Show percentage")
        }
    }
}
