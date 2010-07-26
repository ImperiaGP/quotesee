/*   QuoteSee 0.1.3
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

#ifndef QUOTE_H
#define QUOTE_H

#include <QString>
#include <QList>
#include <QtGui>
#include <Plasma/Svg>
#include <Plasma/IconWidget>
#include <Plasma/DataEngine>
#include "plotter.h"
#include "mrideque.h"

namespace Plasma
{
  class IconWidget;
  class Svg;
}
//namespace QSParent
//{
//    struct Parent
//    {
//        int width;
//        int height;
//    };
//}

class Quote : public QGraphicsWidget
{

    Q_OBJECT

    public:
        Quote(QGraphicsWidget *parent = 0);
        ~Quote();

        virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
        /**
         * @short returns @param name associated with the item
         */
        QString getName() {return name;}
        /**
         * @short returns @param code associated with the item
         */
        QString getCode() {return code;}

    private:
        enum    Status {ERROR, SUCCESS, PROCESSING};
        Status  status;
        QString name;
        QString code;
        QString lastTrade;
        QString change;
        QString lastTradeTime;
        QString lastTradeDate;
        QRectF  frame;
        Plasma::Svg *m_item_background;
        Plasma::IconWidget *m_ext_icon;

//        Plotter *m_plotter;

//        KPlotObject * ob;
//        struct PlotPoint
//        {
//            double axisTime;
//            double actualTime;
//            QString yahooTime;
//            double price;
//        };
//        MRIDeque<PlotPoint> *points;
        int updateNum;
        double minPrice;
        double maxPrice;

    public slots:
        /**
         * @brief updated data, needed for data engine
         * @param source ticker symbol for the source
         * @param data data containing stock symbol, name, last trade,
         *        last trade date, last trade time, change
         */
        void    dataUpdated(const QString &source, const Plasma::DataEngine::Data &data);

    protected slots:
        /**
         * @short displays last trade date and time
         */
//        void    toolTipAboutToShow();
        /**
         * @short opens browser on finance.yahoo.com page for the given ticker symbol
         */
        void    iconClicked();

    protected:
        /**
         * @short hovering over widget will resize frame to make space for icon for opening browser
         */
        virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
        /**
         * will resize frame to previous size
         */
        virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
};
typedef QList<Quote> Quotes;

#endif // QUOTE_H
