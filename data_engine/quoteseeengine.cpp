/*   QuoteSee 0.2.2
 *   Copyright 2009  Jan Zegan <jzegan@gmail.com>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <Plasma/DataContainer>
#include "quoteseeengine.h"
#include "useyql.h"

QuoteseeEngine::QuoteseeEngine(QObject* parent, const QVariantList& args)
    : Plasma::DataEngine(parent, args)
{
    // We ignore any arguments - data engines do not have much use for them
    Q_UNUSED(args);
    manager = new QNetworkAccessManager(this);

    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));

    // YQL Usage limits: http://developer.yahoo.com/yql/
    // Per IP limits: /v1/public/*: 1,000 calls per hour
    // This is relevant when using UseYQL (in CSVtoQuoteList()), there's no info on limits for download.finance.yahoo.com
    // 1 minute - [minutes * seconds * miliseconds]
    setMinimumPollingInterval(1 * 60 * 1000);
}
QuoteseeEngine::~QuoteseeEngine()
{
    delete manager;
    manager = 0;
}

bool QuoteseeEngine::sourceRequestEvent(const QString &name)
{
    setData(name.toLower(), DataEngine::Data());
    return true;
}

bool QuoteseeEngine::updateSourceEvent(const QString &name)
{
    QUrl url("http://download.finance.yahoo.com");

    url.setPath("/d/quotes.csv");
    url.setQueryDelimiters('=', '&');

    // tag explanation: http://www.gummy-stuff.org/Yahoo-data.htm
    // e1 -> error indication: symbol changed or invalid
    // s  -> symbol
    // c1 -> change      <- currency quotes will not have change values, do with yql/xml -> UseYQL
    // l1 -> last trade
    // d1 -> last trade date
    // t1 -> last trade time
    // n  -> name        <- put the name last to avoid pain when company name contains a coma, lazy way

    url.addQueryItem("f", "e1sc1l1d1t1n");
    url.addQueryItem("e", ".csv");
    url.addQueryItem("s", name);

    manager->get(QNetworkRequest(url));

    return true;
}

void QuoteseeEngine::replyFinished(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError)
    {
        qWarning("Request failed, network error");
    }
    else
        CSVtoQuoteList(reply);
}

void QuoteseeEngine::getChangeData(QString code, QString change)
{
    Q_UNUSED(code);
    setData(code, "change", change);
}
void QuoteseeEngine::CSVtoQuoteList(QIODevice *resp)
{
    QString csvContents(resp->readAll());
    QStringList csvContentsByLine = csvContents.split("\r\n", QString::SkipEmptyParts);
    QStringList::ConstIterator stringListIterator;

    for(stringListIterator = csvContentsByLine.begin(); stringListIterator != csvContentsByLine.end(); ++stringListIterator)
    {
        QString code = (*stringListIterator).section(",",1,1).remove("\"").toLower();
        if(!(*stringListIterator).section(",",0,0).startsWith("\"No such ticker"))
        {
            setData(code, "status", true);
            setData(code, "last_trade", (*stringListIterator).section(",",3,3));
            setData(code, "last_trade_date", (*stringListIterator).section(",",4,4).remove("\""));

            setData(code, "code", code);
            setData(code, "last_trade_time", (*stringListIterator).section(",",5,5).remove("\""));
            setData(code, "name", (*stringListIterator).section(",",6,7).remove("\""));

            if(code.endsWith("=x", Qt::CaseInsensitive))
            {
                UseYQL *change = new UseYQL;
                connect(change, SIGNAL(changeReady(QString, QString)), SLOT(getChangeData(QString, QString)));
                change->setCode(code);
            }
            else
            {
                setData(code, "change", (*stringListIterator).section(",",2,2));
            }
        }
        else
        {
            setData(code, "status", false);
            setData(code, "code", code);
            setData(code, "change", "N/A");
            setData(code, "last_trade", "ERROR");
            setData(code, "last_trade_date", "N/A");
            setData(code, "last_trade_time", (*stringListIterator).section(",",5,5).remove("\""));
            setData(code, "name", "Incorrect symbol");

            qWarning() << "No such symbol exists: " << code;
        }
    }
}
// This does the magic that allows Plasma to load
// this plugin.  The first argument must match
// the X-Plasma-EngineName in the .desktop file.
K_EXPORT_PLASMA_DATAENGINE(quotesee, QuoteseeEngine)

// this is needed since QuoteseeEngine is a QObject
#include "quoteseeengine.moc"
