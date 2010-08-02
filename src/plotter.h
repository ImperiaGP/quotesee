#ifndef PLOTTER_H
#define PLOTTER_H

#define AXIS_LENGHT 10

#include <qwt-qt4/qwt_plot.h>
#include <qwt-qt4/qwt_scale_draw.h>
#include <qwt-qt4/qwt_text.h>
#include <QGraphicsWidget>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <Plasma/Svg>

class QGraphicsLinearLayout;

class TimeScaleDraw: public QwtScaleDraw
{
public:
    TimeScaleDraw(const QTime &base): baseTime(base)
    {
    }
    virtual QwtText label(double v) const
    {
        QTime upTime = baseTime.addSecs(60*(int)v);
        return upTime.toString();
    }
private:
    QTime baseTime;
};

class Plotter : public QGraphicsWidget
{
public:
    Plotter(QGraphicsWidget *parent);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
private:
    void addPlot();
    void configPlot();
    void addSampleData();
    QwtPlot *m_plot;
    QGraphicsLinearLayout *m_layout;
    Plasma::Svg *m_item_background;
};

#endif // PLOTTER_H
