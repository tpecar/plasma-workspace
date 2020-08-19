/*
 * Copyright 2020 Carson Black <uhhadd@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 2.10
import QtQuick.Layouts 1.10

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 3.0 as PC3

PC3.Label {
    text: {
        var lines = digitalClock.timeText.split(/[\s:]+/)

        if (digitalClock.showTimezone)
            lines.push(digitalClock.tzText)

        if (digitalClock.showDate)
            lines.push(digitalClock.dateText)

        return lines.join("\n")
    }

    horizontalAlignment: Text.AlignHCenter
    fontSizeMode: Text.HorizontalFit

    Layout.fillWidth: true
    Layout.minimumHeight: implicitHeight
    Layout.maximumWidth: PlasmaCore.Units.gridUnit * 4
    Layout.leftMargin: PlasmaCore.Units.smallSpacing
    Layout.rightMargin: PlasmaCore.Units.smallSpacing
}
