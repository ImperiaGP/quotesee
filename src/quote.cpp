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

#include "quote.h"
#include <QtGui/QtGui>
#include <QtCore/QtGlobal>
#include <QtCore/QTextStream>
#include <Plasma/Theme>
#include <KRun>
#include </usr/include/KDE/Plasma/ToolTipManager>
#include </usr/include/KDE/Plasma/ToolTipContent>
#include <kdebug.h>
#include "plotting/kplotobject.h"
#include "plotting/kplotpoint.h"

Quote::Quote(QGraphicsWidget *parent) :
        QGraphicsWidget(parent),
        status(PROCESSING),
        name("fetching data..."),
        code("please wait"),
        lastTrade("0"),
        change("0"),
        m_item_background(new Plasma::Svg(this)),
        m_ext_icon(new Plasma::IconWidget(this)),
        m_plotter(0),
        ob(0),
        points(0),
        updateNum(0),
        minPrice(0),
        maxPrice(0)
{
    setMinimumHeight(55);
    setMaximumHeight(55);
    setMinimumWidth(270);

    points = new MRIDeque<PlotPoint>(10);

    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    m_plotter = new Plotter(this);
    ob = new KPlotObject(Qt::blue, KPlotObject::Lines, 2, KPlotObject::Circle);
    ob->setShowPoints(true);
    m_plotter->addPlotObject(ob);
    m_plotter->hide();

    // External link icon shows whe hovering over the widget
    setAcceptsHoverEvents(true);

    // display content in a frame,  when mouse hovers over the widget, hoverEnterEvent happens
    // during which frame shrinks to make space for the external link button. Frame grows to previous
    // size when mouse leaves the widget with hoverLeaveEvent
    frame.setTopLeft(QPoint(0, 0));
    frame.setSize(contentsRect().size().toSize());
    frame.adjust(10, 5, -10, -5);

    connect(m_ext_icon, SIGNAL(clicked()), this, SLOT(iconClicked()));

    m_ext_icon->setSvg("quotesee/extIcon");
    // draw icon on top
    m_ext_icon->setZValue(1000);
    // icon is apparently square only
    m_ext_icon->resize(m_ext_icon->sizeFromIconSize(20));
    m_ext_icon->hide();

    qreal width = geometry().width();
    qreal height = geometry().height();
    // show external link icon
    m_ext_icon->setPos(width - m_ext_icon->geometry().width(), (height - m_ext_icon->geometry().height())/2);
}

Quote::~Quote()
{
    delete m_item_background;
    delete m_ext_icon;
}

void Quote::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    m_item_background->setImagePath("quotesee/itemBackground");

    m_item_background->resize(contentsRect().width(), contentsRect().height());
    m_item_background->paint(painter,0, 0);

    QFont font(painter->font());
    painter->setPen(Qt::white);
    font.setBold(true);
    painter->setFont(font);

    painter->drawText(frame, Qt::AlignLeft  | Qt::AlignTop, code.toUpper());

    painter->drawText(frame, Qt::AlignLeft  | Qt::AlignBottom, name);

    QString schange;
    QTextStream stream(&schange);

    switch(status)
    {
        case SUCCESS : {
                            painter->drawText(frame, Qt::AlignRight | Qt::AlignTop, lastTrade);

                            font.setBold(false);
                            painter->setFont(font);

                            stream.setRealNumberNotation(QTextStream::FixedNotation);
                            double rchange = change.toDouble();
                            double pchange = (rchange / (lastTrade.toDouble() - rchange)) * 100;

                            // if currency use 4 number precision, if stock 2
                            stream.setRealNumberPrecision(2);

                            if(!code.contains("=x", Qt::CaseInsensitive))
                            {
                                stream << rchange;
                                stream.setRealNumberPrecision(2);
                                stream << " (" <<  pchange << "%)";
                            }
                            painter->setPen(change.toDouble() < 0 ? Qt::red : Qt::green);
                            painter->drawText(frame, Qt::AlignRight | Qt::AlignBottom, schange);
                            break;
                        }
        case ERROR :    {
                            painter->setPen(Qt::red);
                            painter->drawText(frame, Qt::AlignRight | Qt::AlignBottom, ("ERROR"));
                            break;
                        }
        case PROCESSING :
                        {
                            // nothing to do
                        }
    }
}

void Quote::iconClicked()
{
    QString url = "http://finance.yahoo.com/q?s=";
    url += code.toLower();

    KRun::runUrl(KUrl(url), "text/html", 0);
}

void Quote::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);

    QString content;
    content += i18n("Last trade date: %1 ET<br />", lastTradeDate);
    content += i18n("Last trade time: %1 ET", lastTradeTime);

    m_plotter->setName(code);
    m_plotter->setInfo(content);
    m_plotter->setLabelText(content);
    m_plotter->setPos(0, floor(mapFromParent(0, parentWidget()->size().height()).y()));

    frame.adjust(0, 0, -20, 0);

    m_plotter->show();
    m_ext_icon->show();
    update();
}

void Quote::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);

    frame.adjust(0, 0, 20, 0);
    m_ext_icon->hide();
    m_plotter->hide();
    update();
}

double Quote::toAxisTime(QTime val)
{
    int h = val.hour();
    int m = 100*val.minute()/60;

    return (double)h + (double)m/100;
}

void Quote::makeNewPlot()
{
    QTime time = QTime::fromString(lastTradeTime, "h:mmap");

    PlotPoint pt;
    pt.actualTime = (double)time.hour() + ((double)time.minute())/100;
    pt.axisTime = toAxisTime(time);
    pt.yahooTime = lastTradeTime;
    pt.price = lastTrade.toDouble();

    // should be safe, if deque empty, it won't check the second condition
    if((points->empty()) || (points->at(0).axisTime != pt.axisTime))
    {
        points->push(pt);
    }

    // print contents of deque
    qDebug() << points->count() << "-points-";

    qDebug() <<  QString("%1%2%3%4").arg("actual", -10)
                                    .arg("axis", -10)
                                    .arg("yahoo", -10)
                                    .arg("price", -10);
    foreach(PlotPoint p, *points)
    {
        qDebug() << QString("%1%2%3%4").arg(QString::number(p.actualTime, 'f', 2), -10)
                                       .arg(QString::number(p.axisTime, 'f', 2), -10)
                                       .arg(p.yahooTime,-10)
                                       .arg(p.price, -10);
    }

    if(updateNum == 0)
    {
        minPrice = pt.price;
        maxPrice = pt.price;
    }

    // above will only run for the first update
    if(updateNum % 5 == 0 && updateNum != 1)
    {
        // resetting update number
        updateNum = 1;
    }

    // find max and min values in queue
    foreach(PlotPoint p, *points)
    {
        if(p.price < minPrice)
        {
            minPrice = p.price;
        }
        if(p.price > maxPrice)
        {
            maxPrice = p.price;
        }
    }

    qDebug() << __LINE__ << "minPrice:" << minPrice << "maxPrice:" << maxPrice;
    qDebug() << __LINE__ << "minTime :" << points->last().actualTime
             << "maxTime" << points->first().actualTime;

    // each update paint points from queue, makes it easier to maintain
    ob->clearPoints();

    /* axis limites */
    m_plotter->setLimits(points->last().actualTime-0.006, points->first().actualTime+0.003, minPrice-0.05, maxPrice+0.05);

    foreach(PlotPoint p, *points)
    {
        ob->addPoint(p.axisTime, p.price, QString::number(p.axisTime, 'f', 2));
    }
}

void Quote::dataUpdated(const QString &source, const Plasma::DataEngine::Data &data)
{
    Q_UNUSED(source);

    qDebug("\n[%s:%i] %s()",
           ((strrchr(__FILE__, '/') ? : __FILE__ - 1) + 1),
           __LINE__,
           __FUNCTION__);

    kDebug() << __LINE__ << "************************update:" << updateNum;

    if(!data.empty())
    {
        code          = source;
        status        = (Status)data["status"].toInt();
        name          = data["name"].toString();
        lastTrade     = data["last_trade"].toString();
        change        = data["change"    ].toString();
        lastTradeTime = data["last_trade_time"].toString();
        lastTradeDate = data["last_trade_date"].toString();

    qDebug("  -> %s | %s | %s | %s | %s | %s\n",
           code.toLatin1().data(),
           name.toLatin1().data(),
           lastTrade.toLatin1().data(),
           change.toLatin1().data(),
           lastTradeTime.toLatin1().data(),
           lastTradeDate.toLatin1().data());

    makeNewPlot();

    }
    updateNum++;
    update();
    kDebug() << __LINE__ << "************************update";
}
