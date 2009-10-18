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

#ifndef QUOTESEEENGINE_H
#define QUOTESEEENGINE_H

#include <Plasma/DataEngine>
#include <QXmlStreamReader>
#include <QHttpResponseHeader>
#include <QtNetwork>
#include <QHttp>
#include <QUrl>
#include <QObject>
#include <QThread>

class QuoteseeEngine : public Plasma::DataEngine
{
    Q_OBJECT

    public:
        // every engine needs a constructor with these arguments
        QuoteseeEngine(QObject* parent, const QVariantList& args);
        ~QuoteseeEngine();

    private:
        QNetworkAccessManager *manager;
        void            CSVtoQuoteList(QIODevice *);

    private slots:
        void replyFinished(QNetworkReply* reply);
        void getChangeData(QString, QString);

    protected:
        // this virtual function is called when a new source is requested
        bool sourceRequestEvent(const QString& name);

        // this virtual function is called when an automatic update
        // is triggered for an existing source (ie: when a valid update
        // interval is set when requesting a source)
        bool updateSourceEvent(const QString& source);
};

#endif // QUOTESEEENGINE_H
