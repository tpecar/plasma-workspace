/*
  SPDX-FileCopyrightText: 2021 Han Young <hanyoung@protonmail.com>

  SPDX-License-Identifier: LGPL-3.0-or-later
*/
import QtQuick 2.12
import QtQuick.Controls 2.12

import org.kde.kirigami 2.7 as Kirigami
import org.kde.kcm 1.2

SimpleKCM {
    implicitHeight: 500
    ComboBox {
        id: control
        model: kcm.localeModel
        textRole: "display"
        currentIndex: kcm.globalIndex
        contentItem: Text {
            leftPadding: 0
            rightPadding: control.indicator.width + control.spacing
            text: control.displayText
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
        }
        popup: Popup {
            y: control.height - 1
            width: control.width
            implicitHeight: contentItem.implicitHeight
            padding: 1

            contentItem: ListView {
                clip: true
                implicitHeight: contentHeight
                model: control.popup.visible ? kcm.localeModel : null
                currentIndex: control.highlightedIndex
                delegate: Kirigami.BasicListItem {
                    icon: model.decoration
                    label: model.display
                    highlighted: control.highlightedIndex === index
                    onClicked: {
                        control.popup.close()
                        control.currentIndex = index
                    }
                }
                ScrollIndicator.vertical: ScrollIndicator { }
            }
        }
    }
    
}
