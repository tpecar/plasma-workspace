/* This file is part of the KDE project
   SPDX-FileCopyrightText: 2019 Aleix Pol Gonzalez <aleixpol@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef STARTPLASMA_H
#define STARTPLASMA_H

#include "config-startplasma.h"
#include "kcheckrunning/kcheckrunning.h"
#include <ksplashinterface.h>

extern QTextStream out;

QStringList allServices(const QLatin1String &prefix);
int runSync(const QString &program, const QStringList &args, const QStringList &env = {});
void sourceFiles(const QStringList &files);
void messageBox(const QString &text);

void createConfigDirectory();
void runStartupConfig();
void setupCursor(bool wayland);
void runEnvironmentScripts();
void setupPlasmaEnvironment();
void cleanupPlasmaEnvironment();
void cleanupX11();
bool syncDBusEnvironment();
void setupFontDpi();
QProcess *setupKSplash();
void setupX11();

bool startKDEInit();
bool startPlasmaSession(bool wayland);

void waitForKonqi();

static bool hasSystemdService(const QString &serviceName);
static bool useSystemdBoot();

struct KillBeforeDeleter {
    static inline void cleanup(QProcess *pointer)
    {
        if (pointer)
            pointer->kill();
        delete pointer;
    }
};

#endif
