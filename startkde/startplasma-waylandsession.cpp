/* This file is part of the KDE project
   SPDX-FileCopyrightText: 2019 Aleix Pol Gonzalez <aleixpol@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "startplasma.h"

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    // Boot sequence:
    //
    // kdeinit is used to fork off processes which improves memory usage
    // and startup time.
    //
    // * kdeinit starts klauncher first.
    // * Then kded is started. kded is responsible for keeping the sycoca
    //   database up to date. When an up to date database is present it goes
    //   into the background and the startup continues.
    // * Then kdeinit starts kcminit. kcminit performs initialisation of
    //   certain devices according to the user's settings
    //
    // * Then ksmserver is started which takes control of the rest of the startup sequence

    setupFontDpi();
    QScopedPointer<QProcess, KillBeforeDeleter> ksplash;

    if (!qEnvironmentVariableIsSet("KWIN_RESTART_COUNT")) {
        ksplash.reset(setupKSplash());
    }

    out << "startplasma-waylandsession: Starting up...";

    if (qEnvironmentVariableIsSet("DISPLAY")) {
        setupX11();
    } else {
        qWarning() << "running kwin without Xwayland support";
    }

    if (!syncDBusEnvironment()) {
        out << "Could not sync environment to dbus.\n";
        return 2;
    }

    if (!startPlasmaSession(true))
        return 4;

    // Anything after here is logout
    // It is not called after shutdown/restart
    waitForKonqi();
    out << "startplasma-waylandsession: Shutting down...\n";

    runSync(QStringLiteral("kdeinit5_shutdown"), {});

    cleanupX11();
    out << "startplasma-waylandsession: Done.\n";

    return 0;
}
