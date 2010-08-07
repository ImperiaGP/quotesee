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

#include <QGraphicsView>
#include <QGraphicsScene>

#include <qwt-qt4/qwt_plot_curve.h>

//#include "plotting/kplotobject.h"
//#include "plotting/kplotpoint.h"


//#include <QDateTime>

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
        updateNum(0),
        minPrice(0),
        maxPrice(0)
{
    setMinimumHeight(55);
    setMaximumHeight(55);
    setMinimumWidth(270);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    points = new Coordinates(10);
    m_plotter = new Plotter(this);
    m_plotter->hide();

    stockPlot = new QwtPlotCurve("stock");

    // External link icon shows whe hovering over the widget
    setAcceptsHoverEvents(true);

    connect(m_ext_icon, SIGNAL(clicked()), this, SLOT(iconClicked()));

    m_ext_icon->setSvg("quotesee/extIcon");
    // draw icon on top
    m_ext_icon->setZValue(1000);
    // icon is apparently square only
    m_ext_icon->resize(m_ext_icon->sizeFromIconSize(20));
    m_ext_icon->hide();

    // display content in a frame,  when mouse hovers over the widget, hoverEnterEvent happens
    // during which frame shrinks to make space for the external link button. Frame grows to previous
    // size when mouse leaves the widget with hoverLeaveEvent
    frame.setTopLeft(QPoint(0, 0));
    frame.setSize(contentsRect().size().toSize());
    frame.adjust(10, 5, -10, -5);
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

    // show external link icon
    qreal width = geometry().width();
    qreal height = geometry().height();
    m_ext_icon->setPos(width - m_ext_icon->geometry().width(), (height - m_ext_icon->geometry().height())/2);

    frame.adjust(0, 0, -20, 0);


    // sets position of plot to right below the widget and center
    m_plotter->setPos(mapFromParent((parentWidget()->size().width() - m_plotter->size().width())/2, 0).x(),
                      mapFromParent(0, parentWidget()->size().height()).y());


    m_ext_icon->show();
    m_plotter->show();
    update();
}

void Quote::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);

    frame.adjust(0, 0, 20, 0);

    m_plotter->hide();
    m_ext_icon->hide();
    update();
}


void Quote::dataUpdated(const QString &source, const Plasma::DataEngine::Data &data)
{
    Q_UNUSED(source);

    static int numUpdates = 0;

    qDebug("[%s:%i] %s()",
           ((strrchr(__FILE__, '/') ? : __FILE__ - 1) + 1),
           __LINE__,
           __FUNCTION__);

    kDebug() << __LINE__ << "************************update:" << updateNum;

    if(!data.empty())
    {

        // GRAB DATA //
        code          = source;
        status        = (Status)data["status"].toInt();
        name          = data["name"].toString();
        lastTrade     = data["last_trade"].toString();
        change        = data["change"    ].toString();
        lastTradeTime = data["last_trade_time"].toString();
        lastTradeDate = data["last_trade_date"].toString();
        // END GRAB DATA //

        // IS FIRST UPDATE //
        QTime lttime = QTime::fromString(lastTradeTime, "h:mmap");

        // initial max/min price is just the first value from server
        if(updateNum == 0)
        {
            minPrice = lastTrade.toDouble();
            maxPrice = lastTrade.toDouble();
            axisOrigin = lttime;

            qDebug() << "AXIS ORIGIN:" << axisOrigin;
        }
        qDebug() << "LTTIME: " << lttime;
        // END IS FIRST UPDATE //

        // FIND MIN/MAX Y //
        // find max and min values in queue
//            foreach(double value, points->values)
            for(int i = 0; i < points->values->count(); i++)
        {
            if(points->values->at(i) < minPrice)
            {
                minPrice = points->values->at(i);
//                    kDebug() << __LINE__ << "minPrice: " << minPrice;
            }
            if(points->values->at(i) > maxPrice)
            {
                maxPrice = points->values->at(i);
//                    kDebug() << __LINE__ <<  "maxPrice" << maxPrice;
            }
        }
        // END FIND MIN/MAX Y //
        // SET Y AXIS RANGE //
//        m_plotter->setAxisScale(QwtPlot::yRight, floor(minPrice), ceil(maxPrice));
            m_plotter->configPlot(minPrice, maxPrice, new TimeScaleDraw(axisOrigin));
            qDebug() << "1 setting y range:" << floor(minPrice) << ceil(maxPrice) << "scale:" << axisOrigin.toString();
        // END SET Y AXIS RANGE //

        // IS PREVIOUS POINT AT THE SAME X COORDINATE //
        // should be safe, if deque empty, it won't check the second condition
        // don't add a point if previous time is the same, i.e. data did not change
        if((points->axisTime->empty()) || (points->actualTime->at(0) != lttime))
        {
            points->addCoord(axisOrigin, lttime, lastTrade.toDouble());
//                kDebug() << __LINE__ << "pushing point" << pt.axisTime << pt.price;



            // IS POINT COUNT == 10 //
            if(points->values->count() == 1)
            {
                axisOrigin = lttime;
                kDebug() << "resetting update number, scale: " << lttime;
                reposition(axisOrigin, points);
                // MOVE AXIS ORIGIN //
//                m_plotter->setAxisScaleDraw(QwtPlot::xBottom, new TimeScaleDraw(axisOrigin));
                m_plotter->configPlot(minPrice, maxPrice, new TimeScaleDraw(axisOrigin));
                qDebug() << "2 min/max:" << minPrice << maxPrice << "x scale: " << axisOrigin.toString();
                // END MOVE AXIS ORIGIN //
//                updateNum = 1;
            }
            // END IS POINT COUNT == 10 //

            m_plotter->addGraph(points->axisTime->toVector(), points->values->toVector());
        }
        // END SAME X COORDINATE CHECK

    qDebug("  -> %s | %s | %s | %s | %s | %s\n",
           code.toLatin1().data(),
           name.toLatin1().data(),
           lastTrade.toLatin1().data(),
           change.toLatin1().data(),
           lastTradeTime.toLatin1().data(),
           lastTradeDate.toLatin1().data());

    qDebug() << "points";
    for(int i = 0; i < points->axisTime->count(); i++)
     {
         qDebug() << "  ->(" << points->actualTime->at(i) << points->axisTime->at(i) << ", " << points->values->at(i) << ")";
     }



    }
    m_plotter->replot();
    updateNum++;
    update();
    kDebug() << __LINE__ << "************************update";
}

void Quote::reposition(QTime axisOrigin, Coordinates *points)
{
    points->axisTime->clear();
    qDebug() << "repoisitioned: ";

    for(int i = 0; i < points->axisTime->count(); i++)
    {
        points->axisTime->push((axisOrigin.minute() % 10) + points->actualTime->at(i).minute());
        qDebug() << (axisOrigin.minute() % 10) + points->actualTime->at(i).minute();
    }
}
