/*
  SPDX-FileCopyrightText: 2021 Han Young <hanyoung@protonmail.com>

  SPDX-License-Identifier: LGPL-3.0-or-later
*/
import QtQuick 2.12
import QtQuick.Controls 2.12
import org.kde.kirigami 2.7 as Kirigami
ComboBox {
    id: control
    model: kcm.localeModel
    textRole: "display"
    delegate: Kirigami.BasicListItem {
        width: control.width
        icon: kcm.localeModel.getFlag(index)
        iconSize: Kirigami.Units.gridUnit
        label: kcm.localeModel.getText(index)
        highlighted: control.highlightedIndex === index
        onClicked: {
            control.popup.close()
            currentIndex = index
        }
    }
} 
