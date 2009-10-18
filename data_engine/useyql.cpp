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

#include <QtXmlPatterns>
#include <QXmlQuery>
#include "useyql.h"

UseYQL::UseYQL(QObject *parent) : QObject(parent)
{
    manager = new QNetworkAccessManager(this);
    QObject::connect(manager , SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
}
UseYQL::~UseYQL()
{
    delete manager;
    manager = 0;
}
void UseYQL::replyFinished(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError)
    {
        qWarning("Request failed, network error: ");
        return;
    }
    else
        emit processXML(reply);
}
void UseYQL::setCode(QString code)
{
    m_code = code;
    fetchCurrencyChange();
}
void UseYQL::fetchCurrencyChange()
{
    // enter the line below in http://developer.yahoo.com/yql/console/ to get nodes with change
    // select * from html where url="http://finance.yahoo.com/q?s=<code1>" and (xpath='//span[@id="yfs_c10_<code2>"]/strong')
    // <code1> can be yhoo, in which case <code2> would also be yhoo, however if we're querying a currency
    // <code1> will be for ex. cadusd%3dx and <cod2> cadusd=x
    // can't use QXmlQuery right away, because it interprets a url containing '[' or ']' as xquery and throws error

        QUrl url("http://query.yahooapis.com");
        QString query  = "select * from html where url=\"http://finance.yahoo.com/q?s=";
                query += m_code.replace(QString("="), QString("%3D")).toLower();
                query += "\" and xpath='//span[@id=\"yfs_c10_";
                query += m_code.replace(QString("%3D"), QString("="), Qt::CaseInsensitive).toLower();
                query += "\"]/strong'";

        url.setPath("/v1/public/yql");
        url.setQueryDelimiters('=', '&');
        url.addQueryItem("q", query);
        url.addQueryItem("format", "xml");

        manager->get(QNetworkRequest(url));
}

void UseYQL::processXML(QIODevice *r)
{
    QByteArray out(r->readAll());
    QBuffer buff(&out);
    buff.open(QIODevice::ReadWrite);

    QXmlQuery queryxml;

    // buff will contain a document with 2 exactly the same nodes, select the first one
    queryxml.bindVariable("changeNodes", &buff);
    queryxml.setQuery("doc($changeNodes)/query/results/strong[1]");

    QByteArray output;
    QBuffer buffer(&output);
    buffer.open(QIODevice::WriteOnly);

    QXmlSerializer serializer(queryxml, &buffer);
    Change change = up;

    m_xml.clear();

    queryxml.evaluateTo(&serializer);
    buffer.close();

    m_xml.addData(output);

    // is there a better way to do this?
    while (!m_xml.atEnd())
    {
        m_xml.readNext();
        // change value is stored in this node, sign is determined by class name, there's a space
        // at the beginning of this name
        if(m_xml.attributes().value("class") == " yfi-price-change-down")
        {
            change = down;
            m_xml.readNext();
            assignCurrencyChange(change, m_xml.text().toString());
        }
        else if(m_xml.attributes().value("class") == " yfi-price-change-up")
        {
            m_xml.readNext();
            assignCurrencyChange(change, m_xml.text().toString());
        }
        else if(m_xml.attributes().value("class") == " yfi-price-no-change")
        {
            m_xml.readNext();
            assignCurrencyChange(change, m_xml.text().toString());
        }
    }
    if (m_xml.hasError())
    {
         qWarning() << "error:" << m_xml.errorString();
         m_change = "XML ERROR";
    }

    emit changeReady(m_code, m_change);
    delete r;
    r = 0;
}

void UseYQL::assignCurrencyChange(Change change, QString amount)
{
    if(change == UseYQL::down)
    {
        m_change = "-" + amount;
    }
    else if(change == UseYQL::up)
    {
        m_change = "+" + amount;
    }
}
