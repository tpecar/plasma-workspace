/*
  SPDX-FileCopyrightText: 2021 Han Young <hanyoung@protonmail.com>

  SPDX-License-Identifier: LGPL-3.0-or-later
*/
import QtQuick 2.12
import QtQuick.Controls 2.12
import org.kde.kirigami 2.7 as Kirigami
ComboBox {
        property alias index: control.currentIndex
        id: control
        width: 300
        model: kcm.localeModel
        textRole: "display"
        currentIndex: kcm.langIndex
        delegate: Kirigami.BasicListItem {
                    icon: kcm.localeModel.getFlag(index)
                    label: kcm.localeModel.getText(index)
                    highlighted: control.highlightedIndex === index
                    onClicked: {
                        control.popup.close()
                        control.currentIndex = index
                  }
        }
} 
