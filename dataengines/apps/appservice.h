/*
 * Copyright 2009 Chani Armitage <chani@kde.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Library General Public License version 2 as
 * published by the Free Software Foundation
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef APPSERVICE_H
#define APPSERVICE_H

// plasma
#include <Plasma5Support/Service>
#include <Plasma5Support/ServiceJob>

// own
#include "appsource.h"

/**
 * App Service
 */
class AppService : public Plasma5Support::Service
{
    Q_OBJECT

public:
    explicit AppService(AppSource *source);
    ~AppService() override;

protected:
    Plasma5Support::ServiceJob *createJob(const QString &operation, QMap<QString, QVariant> &parameters) override;

private:
    AppSource *m_source;
};

#endif // APPSERVICE_H
