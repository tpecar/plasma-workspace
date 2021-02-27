/*
 *   SPDX-FileCopyrightText: %{CURRENT_YEAR} %{AUTHOR} <%{EMAIL}>
 *
 *   SPDX-License-Identifier: LGPL-2.0-or-later
 */

#ifndef ION % {APPNAMEUC } _H
#define ION % {APPNAMEUC} _H

#include <plasma/weather/ion.h>

class Q_DECL_EXPORT % {APPNAME} Ion : public IonInterface
{
    Q_OBJECT

public:
    % {APPNAME} Ion(QObject * parent, const QVariantList &args);
    ~ % {APPNAME} Ion();

public: // IonInterface API
    bool updateIonSource(const QString &source) override;
    void reset() override;

private:
    void fetchValidateData(const QString &source);
    void fetchWeatherData(const QString &source);

    void onValidateReport(const QString &source);
    void onWeatherDataReport(const QString &source);
};

#endif
