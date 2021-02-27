/*
ksmserver - the KDE session management server

SPDX-FileCopyrightText: 2000 Matthias Ettrich <ettrich@kde.org>

SPDX-License-Identifier: MIT
*/

#ifndef SHUTDOWNDLG_H
#define SHUTDOWNDLG_H

#include <kquickaddons/quickviewsharedengine.h>
#include <kworkspace.h>
#include <sessionmanagement.h>

namespace KWayland
{
namespace Client
{
class PlasmaShell;
class PlasmaShellSurface;
}
}

// The confirmation dialog
class KSMShutdownDlg : public KQuickAddons::QuickViewSharedEngine
{
    Q_OBJECT

public:
    KSMShutdownDlg(QWindow *parent, KWorkSpace::ShutdownType sdtype, KWayland::Client::PlasmaShell *plasmaShell = nullptr);

    void init();
    bool result() const;

public Q_SLOTS:
    void accept();
    void reject();
    void slotLogout();
    void slotHalt();
    void slotReboot();
    void slotReboot(int);
    void slotSuspend(int);
    void slotLockScreen();

Q_SIGNALS:
    void accepted();
    void rejected();

protected:
    void resizeEvent(QResizeEvent *e) override;
    bool event(QEvent *e) override;

private:
    void setupWaylandIntegration();
    QString m_bootOption;
    QStringList rebootOptions;
    bool m_result : 1;
    SessionManagement m_session;
    KWayland::Client::PlasmaShell *m_waylandPlasmaShell;
    KWayland::Client::PlasmaShellSurface *m_shellSurface = nullptr;
};

#endif
