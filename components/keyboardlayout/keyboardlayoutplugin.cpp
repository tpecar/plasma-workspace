/*
    SPDX-FileCopyrightText: 2014 Martin Yrjölä <martin.yrjola@gmail.com>

    SPDX-License-Identifier: MIT
*/

#include "keyboardlayoutplugin.h"
#include "keyboardlayout.h"

#include <QQmlEngine>

void KeyboardLayoutPlugin::registerTypes(const char *uri)
{
    Q_ASSERT(uri == QLatin1String("org.kde.plasma.workspace.keyboardlayout"));

    qmlRegisterType<KeyboardLayout>(uri, 1, 0, "KeyboardLayout");
}
