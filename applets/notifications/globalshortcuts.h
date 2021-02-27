/*
 * SPDX-FileCopyrightText: 2019 Kai Uwe Broulik <kde@broulik.de>
 *
 * SPDX-License-Identifier: GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
 */

#pragma once

#include <QObject>

class QAction;

class GlobalShortcuts : public QObject
{
    Q_OBJECT

public:
    explicit GlobalShortcuts(QObject *parent = nullptr);
    ~GlobalShortcuts() override;

    Q_INVOKABLE void showDoNotDisturbOsd(bool doNotDisturb) const;

signals:
    void toggleDoNotDisturbTriggered();

private:
    QAction *m_toggleDoNotDisturbAction;
};
