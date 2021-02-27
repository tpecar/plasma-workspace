/*
 *   SPDX-FileCopyrightText: 2010 Ivan Cukic <ivan.cukic(at)kde.org>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "splashwindow.h"
#include "splashapp.h"

#include <KConfigGroup>
#include <KSharedConfig>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QQmlContext>
#include <QQuickItem>
#include <QStandardPaths>
#include <QSurfaceFormat>
#include <QTimer>

#include <KPackage/Package>
#include <KPackage/PackageLoader>

#include <KWayland/Client/plasmashell.h>
#include <KWayland/Client/surface.h>
#include <KWindowSystem>

SplashWindow::SplashWindow(bool testing, bool window, const QString &theme)
    : KQuickAddons::QuickViewSharedEngine()
    , m_stage(0)
    , m_testing(testing)
    , m_window(window)
    , m_theme(theme)
{
    setColor(Qt::transparent);
    setDefaultAlphaBuffer(true);
    setClearBeforeRendering(true);
    setResizeMode(KQuickAddons::QuickViewSharedEngine::SizeRootObjectToView);

    if (!m_window) {
        setFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    }

    if (!m_testing && !m_window) {
        if (KWindowSystem::isPlatformX11()) {
            // X11 specific hint only on X11
            setFlags(Qt::BypassWindowManagerHint);
        } else if (!KWindowSystem::isPlatformWayland()) {
            // on other platforms go fullscreen
            // on Wayland we cannot go fullscreen due to QTBUG 54883
            setWindowState(Qt::WindowFullScreen);
        }
    }

    if (m_testing && !m_window && !KWindowSystem::isPlatformWayland()) {
        setWindowState(Qt::WindowFullScreen);
    }

    // be sure it will be eventually closed
    // FIXME: should never be stuck
    QTimer::singleShot(30000, this, &QWindow::close);
}

void SplashWindow::setStage(int stage)
{
    m_stage = stage;

    rootObject()->setProperty("stage", stage);
}

bool SplashWindow::event(QEvent *e)
{
    if (e->type() == QEvent::PlatformSurface) {
        auto pe = static_cast<QPlatformSurfaceEvent *>(e);
        switch (pe->surfaceEventType()) {
        case QPlatformSurfaceEvent::SurfaceCreated:
            setupWaylandIntegration();
            break;
        case QPlatformSurfaceEvent::SurfaceAboutToBeDestroyed:
            delete m_shellSurface;
            m_shellSurface = nullptr;
            break;
        }
    }
    return KQuickAddons::QuickViewSharedEngine::event(e);
}

void SplashWindow::keyPressEvent(QKeyEvent *event)
{
    KQuickAddons::QuickViewSharedEngine::keyPressEvent(event);
    if (m_testing && !event->isAccepted() && event->key() == Qt::Key_Escape) {
        close();
    }
}

void SplashWindow::mousePressEvent(QMouseEvent *event)
{
    KQuickAddons::QuickViewSharedEngine::mousePressEvent(event);
    if (m_testing && !event->isAccepted()) {
        close();
    }
}

void SplashWindow::setGeometry(const QRect &rect)
{
    bool oldGeometryEmpty = geometry().isNull();
    KQuickAddons::QuickViewSharedEngine::setGeometry(rect);

    if (oldGeometryEmpty) {
        KPackage::Package package = KPackage::PackageLoader::self()->loadPackage(QStringLiteral("Plasma/LookAndFeel"));
        KConfigGroup cg(KSharedConfig::openConfig(), "KDE");
        const QString packageName = cg.readEntry("LookAndFeelPackage", QString());
        if (!packageName.isEmpty()) {
            package.setPath(packageName);
        }

        if (!m_theme.isEmpty()) {
            package.setPath(m_theme);
        }

        Q_ASSERT(package.isValid());
        setSource(QUrl::fromLocalFile(package.filePath("splashmainscript")));
    }

    if (m_shellSurface) {
        m_shellSurface->setPosition(geometry().topLeft());
    }
}

void SplashWindow::setupWaylandIntegration()
{
    if (m_shellSurface) {
        // already setup
        return;
    }
    if (SplashApp *a = qobject_cast<SplashApp *>(qApp)) {
        using namespace KWayland::Client;
        PlasmaShell *interface = a->waylandPlasmaShellInterface();
        if (!interface) {
            return;
        }
        Surface *s = Surface::fromWindow(this);
        if (!s) {
            return;
        }
        m_shellSurface = interface->createSurface(s, this);
        // Use OSD to make it go above all other windows
        // that's the closest we have to the X11 unmanged layer we have on Wayland
        m_shellSurface->setRole(PlasmaShellSurface::Role::OnScreenDisplay);
        m_shellSurface->setPosition(geometry().topLeft());
    }
}
