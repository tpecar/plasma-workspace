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
    implicitHeight: 500
    ColumnLayout {
        RowLayout {
            Label {
                text: i18n("Region:")
            }
            FormatComboBox {
                index: kcm.langIndex
            }
        }
        
        CheckBox {
            id: detailCheckBox
            text: i18n("detail")
        }
        
        RowLayout {
            Label {
                enabled: detailCheckBox.checked
                text: i18n("Numbers:")
                color: enabled ? Kirigami.Theme.textColor : Kirigami.Theme.disabledTextColor
            }
            FormatComboBox {
                enabled: detailCheckBox.checked
                index: kcm.numericIndex
            }
        }
        
        RowLayout {
            Label {
                enabled: detailCheckBox.checked
                text: i18n("Time:")
                color: enabled ? Kirigami.Theme.textColor : Kirigami.Theme.disabledTextColor
            }
            FormatComboBox {
                enabled: detailCheckBox.checked
                index: kcm.timeIndex
            }
        }
        
        RowLayout {
            Label {
                enabled: detailCheckBox.checked
                text: i18n("Currency:")
                color: enabled ? Kirigami.Theme.textColor : Kirigami.Theme.disabledTextColor
            }
            FormatComboBox {
                enabled: detailCheckBox.checked
                index: kcm.monetaryIndex
            }
        }
        
        RowLayout {
            Label {
                enabled: detailCheckBox.checked
                text: i18n("Measurement Units:")
                color: enabled ? Kirigami.Theme.textColor : Kirigami.Theme.disabledTextColor
            }
            FormatComboBox {
                enabled: detailCheckBox.checked
                index: kcm.measurementIndex
            }
        }
        RowLayout {
            Label {
                enabled: detailCheckBox.checked
                text: i18n("Collation and Sorting:")
                color: enabled ? Kirigami.Theme.textColor : Kirigami.Theme.disabledTextColor
            }
            FormatComboBox {
                enabled: detailCheckBox.checked
                index: kcm.collateIndex
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
            text: i18n("Measurement Units: ") + kcm.measurementSetting
        }
    }
}
