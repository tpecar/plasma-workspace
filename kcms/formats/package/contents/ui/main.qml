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
    implicitHeight: formLayout.height + footer.height + Kirigami.Units.gridUnit * 3
    Kirigami.FormLayout {
        id: formLayout
        width: parent.width
        FormatComboBox {
            id: langCombo
            Kirigami.FormData.label: i18n("Region: ")
            Layout.fillWidth: true
            currentIndex: kcm.langIndex
            onCurrentIndexChanged: kcm.langIndex = currentIndex
        }

        
        CheckBox {
            id: detailCheckBox
            checked: kcm.detailed
            onCheckedChanged: if (kcm.detailed != checked) kcm.detailed = checked
            Kirigami.FormData.label: i18n("Detailed Settings ")
        }
        
        FormatComboBox {
            id: numericCombo
            Kirigami.FormData.label: i18n("Numbers: ")
            Layout.fillWidth: true
            enabled: detailCheckBox.checked
            Kirigami.FormData.enabled: detailCheckBox.checked
            currentIndex: kcm.numericIndex
            displayText: !detailCheckBox.checked ? i18n("No change") : currentText
            onCurrentIndexChanged: kcm.numericIndex = currentIndex
        }

        
        FormatComboBox {
            id: timeCombo
            Kirigami.FormData.label: i18n("Time: ")
            Layout.fillWidth: true
            enabled: detailCheckBox.checked
            currentIndex: kcm.timeIndex
            displayText: !detailCheckBox.checked ? i18n("No change") : currentText
            onCurrentIndexChanged: kcm.timeIndex = currentIndex
        }

        

        FormatComboBox {
            id: monetaryCombo
            Kirigami.FormData.label: i18n("Currency: ")
            Layout.fillWidth: true
            enabled: detailCheckBox.checked
            currentIndex: kcm.monetaryIndex
            displayText: !detailCheckBox.checked ? i18n("No change") : currentText
            onCurrentIndexChanged: kcm.monetaryIndex = currentIndex
        }

        
        FormatComboBox {
            id: measurementCombo
            Kirigami.FormData.label: i18n("Measurement Units: ")
            Layout.fillWidth: true
            enabled: detailCheckBox.checked
            currentIndex: kcm.measurementIndex
            displayText: !detailCheckBox.checked ? i18n("No change") : currentText
            onCurrentIndexChanged: kcm.measurementIndex = currentIndex
        }


        FormatComboBox {
            id: collateCombo
            Kirigami.FormData.label: i18n("Collation and Sorting: ")
            Layout.fillWidth: true
            enabled: detailCheckBox.checked
            currentIndex: kcm.collateIndex
            displayText: !detailCheckBox.checked ? i18n("No change") : currentText
            onCurrentIndexChanged: kcm.collateIndex = currentIndex
        }

        Kirigami.Separator {
            Kirigami.FormData.label: i18n("Description")
            Kirigami.FormData.isSection: true
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


    footer: Kirigami.InlineMessage {
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
