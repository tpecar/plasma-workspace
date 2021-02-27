/*
 *   SPDX-FileCopyrightText: 2007 Glenn Ergeerts <glenn.ergeerts@telenet.be>
 *   SPDX-FileCopyrightText: 2012 Glenn Ergeerts <marco.gulino@gmail.com>
 *
 *   SPDX-License-Identifier: LGPL-2.0-or-later
 */

#ifndef FIREFOX_H
#define FIREFOX_H

#include "browser.h"
#include <QSqlDatabase>

class Favicon;
class FetchSqlite;
class Firefox : public QObject, public Browser
{
    Q_OBJECT
public:
    explicit Firefox(QObject *parent = nullptr);
    ~Firefox() override;
    QList<BookmarkMatch> match(const QString &term, bool addEverything) override;
public Q_SLOTS:
    void teardown() override;
    void prepare() override;

private:
    virtual void reloadConfiguration();
    QString m_dbFile;
    QString m_dbFile_fav;
    QString m_dbCacheFile;
    QString m_dbCacheFile_fav;
    Favicon *m_favicon;
    FetchSqlite *m_fetchsqlite;
    FetchSqlite *m_fetchsqlite_fav;
};

#endif // FIREFOX_H
