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

#ifndef USEYQL_H
#define USEYQL_H

#include <QXmlStreamReader>
#include <QHttpResponseHeader>
#include <QtNetwork>
#include <QHttp>
#include <QUrl>

class UseYQL : public QObject
{
Q_OBJECT
public:
    UseYQL(QObject *parent = 0);
    ~UseYQL();
    void setCode(QString);

private:
    enum Change {down, up};
    void assignCurrencyChange(Change change, QString amount);
    QString m_change;
    void fetchCurrencyChange();
    QXmlStreamReader m_xml;
    QNetworkReply *reply;
    QNetworkAccessManager *manager;
    QString m_code;
signals:
    void changeReady(QString, QString);
private slots:
    void processXML(QIODevice *resp);
    void replyFinished(QNetworkReply *);
};

#endif // USEYQL_H
