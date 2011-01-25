#include "plotter.h"
#include "plotting/kplotwidget.h"
#include "plotting/kplotobject.h"
#include "plotting/kplotaxis.h"
#include "plotting/kplotpoint.h"

#include <QGraphicsLinearLayout>
#include <kdebug.h>
#include <QGraphicsScene>
#include <QGraphicsProxyWidget>

#include <plasma/svg.h>
#include "plasma/theme.h"
#include <QtGui>
#include <QLabel>


using namespace::std;

Plotter::Plotter(QGraphicsWidget *parent) :
        QGraphicsWidget(parent),
        m_layout(0),
        m_plot(0),
        m_item_background(new Plasma::Svg(this))
{
    m_layout = new QGraphicsLinearLayout(Qt::Vertical, this);
    m_layout->addStretch();

    addInfoLabel();

    addPlot();
    configPlot();

    setLayout(m_layout);
}
void Plotter::setLabelText(QString text)
{
    infoLabel->setText(text);
}

void Plotter::addInfoLabel()
{
    infoLabel = new QLabel();
    infoLabel->setTextFormat(Qt::RichText);
    infoLabel->setMinimumHeight(35);

    QGraphicsProxyWidget *proxy;
    QGraphicsScene scene;
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout;
    QGraphicsWidget *form = new QGraphicsWidget;

    proxy = scene.addWidget(infoLabel);
    layout->addItem(proxy);
    form->setLayout(layout);

    m_layout->addItem(form);
}

void Plotter::addPlot()
{
    m_plot = new KPlotWidget();

    QGraphicsProxyWidget *proxy;
    QGraphicsScene scene;
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout;
    QGraphicsWidget *form = new QGraphicsWidget;

    proxy = scene.addWidget(m_plot);
    layout->addItem(proxy);
    form->setLayout(layout);

    m_layout->addItem(form);
}

void Plotter::configPlot()
{
    m_plot->setBackgroundColor(QColor(255, 255, 255) );
    m_plot->setForegroundColor(QColor(0, 0, 0));
    m_plot->setMinimumSize( 400, 300 );
    m_plot->setRightPadding(50);
    m_plot->setAntialiasing( true );

    m_plot->axis(KPlotWidget::BottomAxis)->setTickLabelFormat('t');
    m_plot->axis(KPlotWidget::RightAxis)->setTickLabelsShown(true);
    m_plot->axis(KPlotWidget::BottomAxis)->setTickLabelsShown(true);
    m_plot->axis(KPlotWidget::LeftAxis)->setTickLabelsShown(false);
    m_plot->setShowGrid(true);
}

double Plotter::toAxisTime(double val)
{
    int h = int(val);
    int m = int(10000 * (val + 0.005 - h)/60);
    return (double)h + (double)m/100;
}


QList< KPlotObject* > Plotter::plotObjects() const
{
    return m_plot->plotObjects();
}

double Plotter::axisStart(double currentTime, int span)
{
    int h = int(currentTime);
    int m = int( 100.*(currentTime+0.005 - h) ) - span;
    if(m < 0)
    {
        h -= 1;
        m = m + 60;
    }
    return (double)h + (double)m/100;
}

void Plotter::setLimits( double x1, double x2, double xLength, double y1, double y2)
{
    double length = axisStart(x2, xLength);

    qDebug() << QString("%1%2%3%4").arg("x1", -6)
            .arg("x2", -6)
            .arg("xLength", -10)
            .arg("length", -10);
    qDebug() << QString("%1%2%3%4").arg(x1, -6)
            .arg(x2, -6)
            .arg(xLength, -10)
            .arg(length, -10);

    m_plot->setLimits( toAxisTime(length < x1 ? length : x1), toAxisTime(x2 + 0.01), y1, y2 );
}

void Plotter::addPlotObject(KPlotObject *object)
{
    m_plot->addPlotObject(object);
}

void Plotter::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QRectF frame;

    frame.setTopLeft(QPoint(0, 0));
    frame.setSize(contentsRect().size().toSize());

    m_item_background->setImagePath("quotesee/itemBackground");

    m_item_background->resize(contentsRect().width(), contentsRect().height());
    m_item_background->paint(painter,0, 0);
}
