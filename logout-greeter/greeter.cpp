/*
ksmserver - the KDE session management server

SPDX-FileCopyrightText: 2016 Martin Graesslin <mgraesslin@kde.org>
SPDX-FileCopyrightText: 2018 David Edmundson <davidedmundson@kde.org>

SPDX-License-Identifier: MIT
*/

#include "greeter.h"

#include <QApplication>
#include <QDebug>
#include <QScreen>

#include "shutdowndlg.h"

#include "ksmserveriface.h"
#include "logoutpromptadaptor.h"

#include <KQuickAddons/QtQuickSettings>
#include <KWindowSystem>

#include <KWayland/Client/connection_thread.h>
#include <KWayland/Client/plasmashell.h>
#include <KWayland/Client/registry.h>

Greeter::Greeter()
    : QObject()
    , m_waylandPlasmaShell(nullptr)
{
    new LogoutPromptAdaptor(this);
    QDBusConnection::sessionBus().registerObject(QStringLiteral("/LogoutPrompt"), this);
    QDBusConnection::sessionBus().registerService(QStringLiteral("org.kde.LogoutPrompt"));
}

Greeter::~Greeter()
{
    qDeleteAll(m_dialogs);
}

void Greeter::setupWaylandIntegration()
{
    if (!KWindowSystem::isPlatformWayland()) {
        return;
    }
    using namespace KWayland::Client;
    ConnectionThread *connection = ConnectionThread::fromApplication(this);
    if (!connection) {
        return;
    }
    Registry *registry = new Registry(this);
    registry->create(connection);
    connect(registry, &Registry::plasmaShellAnnounced, this, [this, registry](quint32 name, quint32 version) {
        m_waylandPlasmaShell = registry->createPlasmaShell(name, version, this);
    });
    registry->setup();
    connection->roundtrip();
}

void Greeter::init()
{
    setupWaylandIntegration();
    const auto screens = qApp->screens();
    for (QScreen *screen : screens) {
        adoptScreen(screen);
    }
    connect(qApp, &QGuiApplication::screenAdded, this, &Greeter::adoptScreen);
    m_running = true;
}

void Greeter::adoptScreen(QScreen *screen)
{
    // TODO: last argument is the theme, maybe add command line option for it?
    KSMShutdownDlg *w = new KSMShutdownDlg(nullptr, m_shutdownType, m_waylandPlasmaShell);
    w->installEventFilter(this);
    m_dialogs << w;

    QObject::connect(screen, &QObject::destroyed, w, [w, this] {
        m_dialogs.removeOne(w);
        w->deleteLater();
    });
    connect(w, &KSMShutdownDlg::rejected, this, &Greeter::rejected);
    connect(w, &KSMShutdownDlg::accepted, this, []() {
        QApplication::exit(1);
    });
    w->setScreen(screen);
    w->setGeometry(screen->geometry());
    w->init();
}

void Greeter::rejected()
{
    QApplication::exit(1);
}

bool Greeter::eventFilter(QObject *watched, QEvent *event)
{
    if (qobject_cast<KSMShutdownDlg *>(watched)) {
        if (event->type() == QEvent::MouseButtonPress) {
            // check that the position is on no window
            QMouseEvent *me = static_cast<QMouseEvent *>(event);
            for (auto it = m_dialogs.constBegin(); it != m_dialogs.constEnd(); ++it) {
                if ((*it)->geometry().contains(me->globalPos())) {
                    return false;
                }
            }
            // click outside, close
            rejected();
        }
    }
    return false;
}

void Greeter::promptLogout()
{
    if (m_running) {
        return;
    }
    m_shutdownType = KWorkSpace::ShutdownTypeNone;
    init();
}

void Greeter::promptShutDown()
{
    if (m_running) {
        return;
    }
    m_shutdownType = KWorkSpace::ShutdownTypeHalt;
    init();
}

void Greeter::promptReboot()
{
    if (m_running) {
        return;
    }
    m_shutdownType = KWorkSpace::ShutdownTypeReboot;
    init();
}
