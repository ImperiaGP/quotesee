#include <QTime>
#include <QtDebug>
#include <qwt-qt4/qwt_scale_widget.h>
#include <qwt-qt4/qwt_plot_marker.h>
#include <qwt-qt4/qwt_plot_curve.h>
#include <qwt-qt4/qwt_legend.h>
#include <qwt-qt4/qwt_symbol.h>
#include <qwt-qt4/qwt_data.h>

#include <QGraphicsScene>
#include <QGraphicsLinearLayout>
#include <QGraphicsProxyWidget>
#include <QWidget>

#include "plotter.h"

#include <QDebug>

Plotter::Plotter(QGraphicsWidget *parent) :
        QGraphicsWidget(parent),
        m_plot(0),
        m_layout(0),
        m_item_background(new Plasma::Svg(this))
{
    addPlot();
//    configPlot();
//    addSampleData();

    resize(400, 300);
    setLayout(m_layout);

}

void Plotter::addPlot()
{
    m_plot = new QwtPlot;
    m_layout = new QGraphicsLinearLayout(Qt::Vertical, this);
    m_layout->addStretch();

    QGraphicsScene scene;
    QGraphicsProxyWidget *proxy = scene.addWidget(m_plot);

    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout;
    layout->addItem(proxy);

    QGraphicsWidget *form = new QGraphicsWidget;
    form->setLayout(layout);

    m_layout->addItem(form);

}
void Plotter::setAxisScaleDraw(int axisId, QwtScaleDraw *scale)
{
    m_plot->setAxisScaleDraw(axisId, scale);
}
void Plotter::setAxisScale(int axisId, double min, double max, double step)
{
    qDebug( ) << "setAxisScale:" << axisId << min << max << step;
    m_plot->setAxisScale(axisId, min, max, step);
}
void Plotter::replot()
{
    m_plot->replot();
}

void Plotter::configPlot(double min, double max, QwtScaleDraw *scale)
{
//    m_plot->setMinimumSize(300, 250);

    m_plot->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
//    setAxisScale(QwtPlot::xBottom, AXIS_LENGHT, (double)QTime::currentTime().minute(), 1); // length of the scale
    m_plot->setAxisLabelAlignment(QwtPlot::xBottom, Qt::AlignRight | Qt::AlignBottom);



    m_plot->setAxisScale(QwtPlot::yRight, floor(min), ceil(max));
    m_plot->setAxisScaleDraw(QwtPlot::xBottom, scale);
    qDebug() << "y scale" << min << max;


    QwtScaleWidget *scaleWidget = m_plot->axisWidget(QwtPlot::xBottom);
    const int fmh = QFontMetrics(scaleWidget->font()).height();
    scaleWidget->setMinBorderDist(0, fmh / 2);
    m_plot->setAxisLabelRotation(QwtPlot::xBottom, -50.0);
    m_plot->setAxisLabelAlignment(QwtPlot::xBottom, Qt::AlignLeft | Qt::AlignBottom);

    // Set axis titles
    m_plot->enableAxis(QwtPlot::yLeft, false);
    m_plot->enableAxis(QwtPlot::yRight);
//    setAxisTitle(QwtPlot::yRight, "Price");

}
void Plotter::addGraph(const QVector<double> &x, const QVector<double> &y)
{
    QwtSymbol sym;
    sym.setStyle(QwtSymbol::Ellipse);
    sym.setPen(QColor(Qt::blue));
    sym.setBrush(QColor(Qt::yellow));
    sym.setSize(5);
    // Insert new curves
    stockPlot = new QwtPlotCurve("stock");
    stockPlot->setSymbol(sym);

    stockPlot->detach();

    stockPlot->setPen(QPen(Qt::blue));
    stockPlot->attach(m_plot);

    stockPlot->setData(x, y);
}

void Plotter::addSampleData()
{
    QwtSymbol sym;
    sym.setStyle(QwtSymbol::Ellipse);
    sym.setPen(QColor(Qt::blue));
    sym.setBrush(QColor(Qt::yellow));
    sym.setSize(5);
    // Insert new curves
    stockPlot = new QwtPlotCurve("stock");
    stockPlot->setSymbol(sym);


    m_plot->setAxisScale(QwtPlot::yRight, 1.39, 8.32);
    m_plot->setAxisScaleDraw(QwtPlot::xBottom, new TimeScaleDraw(QTime::currentTime()));        // time scale with starting point

    #if QT_VERSION >= 0x040000
        stockPlot->setRenderHint(QwtPlotItem::RenderAntialiased);
    #endif

    QTime now = QTime::currentTime();


    QVector<double> *x = new QVector<double>();
QTime temp;

for(int i = 0; i < 7; i++)
{
    temp = QTime::currentTime().addSecs(i*60);
    int m = temp.minute();
    if(m  < now.minute())
    {
        x->append((double)(now.minute() % 10) + m);
        qDebug() << temp << now.hour() << m << now.minute() << (now.minute() % 10) + m;
    }
    else
    {
        x->append((double)(m - now.minute()));
        qDebug() << temp << m << now.minute() << m - now.minute();
    }
//                x->append((double)QTime::currentTime().addSecs(i*60).minute());

}

QVector<double> *y = new QVector<double>();
y->append(2.23);
y->append(5.23);
y->append(2.96);
y->append(8.32);
y->append(7.12);
y->append(6.3);
y->append(1.39);

stockPlot->setPen(QPen(Qt::blue));
stockPlot->attach(m_plot);
stockPlot->setData(*x, *y);
}

void Plotter::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->save();

    QRectF frame;

    frame.setTopLeft(QPoint(0, 0));
    frame.setSize(contentsRect().size().toSize());

    m_item_background->setImagePath("quotesee/itemBackground");

    m_item_background->resize(contentsRect().width(), contentsRect().height());
    m_item_background->paint(painter,0, 0);

    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    painter->setRenderHint(QPainter::Antialiasing);

    painter->restore();
    update();
}
