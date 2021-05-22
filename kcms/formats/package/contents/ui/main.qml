/*
  SPDX-FileCopyrightLabel: 2021 Han Young <hanyoung@protonmail.com>

  SPDX-License-Identifier: LGPL-3.0-or-later
*/
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.15

import org.kde.kirigami 2.7 as Kirigami
import org.kde.kcm 1.2

SimpleKCM {
    id: root
    implicitHeight: 500
    Kirigami.FormLayout {
        RowLayout {
            Label {
                text: i18n("Region:")
            }
            FormatComboBox {
                id: langCombo
                currentIndex: kcm.langIndex
                onCurrentIndexChanged: kcm.langIndex = currentIndex
            }
        }
        
        CheckBox {
            id: detailCheckBox
            checked: kcm.detailed
            onCheckedChanged: if (kcm.detailed != checked) kcm.detailed = checked
            text: i18n("detail")
        }
        
        RowLayout {
            Label {
                enabled: detailCheckBox.checked
                text: i18n("Numbers:")
                color: enabled ? Kirigami.Theme.textColor : Kirigami.Theme.disabledTextColor
            }
            FormatComboBox {
                id: numericCombo
                enabled: detailCheckBox.checked
                currentIndex: kcm.numericIndex
                displayText: !detailCheckBox.checked ? i18n("No change") : currentText
                onCurrentIndexChanged: kcm.numericIndex = currentIndex
            }
        }
        
        RowLayout {
            Label {
                enabled: detailCheckBox.checked
                text: i18n("Time:")
                color: enabled ? Kirigami.Theme.textColor : Kirigami.Theme.disabledTextColor
            }
            FormatComboBox {
                id: timeCombo
                enabled: detailCheckBox.checked
                currentIndex: kcm.timeIndex
                displayText: !detailCheckBox.checked ? i18n("No change") : currentText
                onCurrentIndexChanged: kcm.timeIndex = currentIndex
            }
        }
        
        RowLayout {
            Label {
                enabled: detailCheckBox.checked
                text: i18n("Currency:")
                color: enabled ? Kirigami.Theme.textColor : Kirigami.Theme.disabledTextColor
            }
            FormatComboBox {
                id: monetaryCombo
                enabled: detailCheckBox.checked
                currentIndex: kcm.monetaryIndex
                displayText: !detailCheckBox.checked ? i18n("No change") : currentText
                onCurrentIndexChanged: kcm.monetaryIndex = currentIndex
            }
        }
        
        RowLayout {
            Label {
                enabled: detailCheckBox.checked
                text: i18n("Measurement Units:")
                color: enabled ? Kirigami.Theme.textColor : Kirigami.Theme.disabledTextColor
            }
            FormatComboBox {
                id: measurementCombo
                enabled: detailCheckBox.checked
                currentIndex: kcm.measurementIndex
                displayText: !detailCheckBox.checked ? i18n("No change") : currentText
                onCurrentIndexChanged: kcm.measurementIndex = currentIndex
            }
        }
        RowLayout {
            Label {
                enabled: detailCheckBox.checked
                text: i18n("Collation and Sorting:")
                color: enabled ? Kirigami.Theme.textColor : Kirigami.Theme.disabledTextColor
            }
            FormatComboBox {
                id: collateCombo
                enabled: detailCheckBox.checked
                currentIndex: kcm.collateIndex
                displayText: !detailCheckBox.checked ? i18n("No change") : currentText
                onCurrentIndexChanged: kcm.collateIndex = currentIndex
            }
        }

        Label {
            text: i18n("Description")
            font.bold: true
        }
        Label {
            text: i18n("Numbers: ") + kcm.numberExample
        }
        Label {
            text: i18n("Time: ") + kcm.timeExample
        }
        Label {
            text: i18n("Currency: ") + kcm.currencyExample
        }
        Label {
            text: i18n("Measurement Units: ") + kcm.measurementExample
        }
    }


    header: Kirigami.InlineMessage {
        id: message
        text: i18n("Your changes will take effect the next time you log in.")
        type: Kirigami.MessageType.Positive
    }
    Connections {
        target: kcm
        function onSaveClicked() {
            message.visible = true
        }
    }

}
