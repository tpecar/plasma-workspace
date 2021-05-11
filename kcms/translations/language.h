// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
// SPDX-FileCopyrightText: 2021 Harald Sitter <sitter@kde.org>

#pragma once

#include <QObject>

class Language : public QObject
{
    Q_OBJECT
    Q_PROPERTY(State state MEMBER state NOTIFY stateChanged)
public:
    enum class State {
        Complete, // already complete, mustn't call complete()
        Missing, // this is a fake language it's not actually available, it was installed at some point but now it is no more
        Incomplete, // the language is installed but lacks packages to be considered complete
        Installing, // the language is currently installing associated packages to complete itself
    };
    Q_ENUM(State);

    explicit Language(QString code_, QObject *parent = nullptr)
        : Language(std::move(code_), State::Complete, parent)
    {
    }

    explicit Language(QString code_, State state_, QObject *parent = nullptr)
        : QObject(parent)
        , code(std::move(code_))
        , state(state_)
    {
    }

    const QString code;
    State state = State::Complete;
    QStringList packages = {};

    void reloadCompleteness(); // only used by model, not invokable from qml
    Q_INVOKABLE void complete(); // called by qml when completion is requested

Q_SIGNALS:
    void stateChanged();
};
