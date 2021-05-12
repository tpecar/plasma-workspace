/*
  SPDX-FileCopyrightText: 2021 Han Young <hanyoung@protonmail.com>

  SPDX-License-Identifier: LGPL-3.0-or-later
*/
import QtQuick 2.12
import QtQuick.Controls 2.12 as Controls

import org.kde.kirigami 2.7 as Kirigami
import org.kde.kcm 1.2

SimpleKCM {
    
    ListView {
        height: 500
        width: 200
        model: kcm.LocaleListModel
        delegate: Controls.Label {
            text: display
        }
    }
}
