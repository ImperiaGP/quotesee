#include "plotter.h"
#include "plotting/kplotwidget.h"
#include "plotting/kplotobject.h"
#include "plotting/kplotaxis.h"

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
//    addPlotPoints();

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
//    setContentsMargins(0, 20, 0, 0);

    m_plot->setShowGrid( false ); // no grid please
    m_plot->setBackgroundColor(QColor(255, 255, 255) );
    m_plot->setForegroundColor(QColor(0, 0, 0));
//    m_plot->setLimits(0.0, 5.0, -1.0, 5.0 );
    m_plot->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
//    m_plot->setMaximumSize(270, 100);
//    m_plot->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    m_plot->setLeftPadding(5);
    m_plot->setRightPadding(40);
    m_plot->setBottomPadding(25);
    m_plot->setTopPadding(5);
    m_plot->axis(KPlotWidget::RightAxis)->setTickLabelsShown(true);
    m_plot->axis(KPlotWidget::BottomAxis)->setTickLabelsShown(true);
//    m_plot->axis(KPlotWidget::BottomAxis)->setTickMarks(0.0, 0.0);
    m_plot->axis(KPlotWidget::TopAxis)->setTickLabelsShown(false);
    m_plot->axis(KPlotWidget::LeftAxis)->setTickLabelsShown(false);

    m_plot->axis(KPlotWidget::RightAxis)->setTickMarks(0.0, 0.0);
    m_plot->axis(KPlotWidget::BottomAxis)->setTickMarks(0.0, 0.0);
    m_plot->setShowGrid(true);
    m_plot->axis(KPlotWidget::BottomAxis)->setTickLabelFormat('t');
//    m_plot->axis(KPlotWidget::RightAxis)->setLabel("price");
    resize( 400, 200);
}

void Plotter::addPlotPoints()
{
//    // Create plot object
//    KPlotObject * ob = new KPlotObject(Qt::red, KPlotObject::Lines, 2, KPlotObject::Circle);
//    // Add plot object to chart
//    m_plot->addPlotObject(ob);
//    ob->setShowPoints(true);
//    // Add some random points to the plot object
//    double s = 0;
////    srand(time(NULL));
//    ob->addPoint( QPoint(0, 0) );
//    for (unsigned int i=0; i<=5; i += 1) {
//            s = rand()*(4.9)/RAND_MAX;
//            ob->addPoint( QPoint(i, s), QString::number(s).left(4));
//            kDebug() << "Adding point [" << i << "," << s << "]" << endl;
//    }
}

QList< KPlotObject* > Plotter::plotObjects() const
{
    return m_plot->plotObjects();
}

void Plotter::setLimits( double x1, double x2, double y1, double y2 )
{
//    double inc;

//    double *yp1 = &y1, *yp2 = &y2;
//    double *xp1 = &x1, *xp2 = &x2;
//    AxisScale::DefineAxis(yp1, yp2, &inc);

    m_plot->setLimits(x1, x2, y1, y2);
//    kDebug() << "major tick marks:" << m_plot->axis(KPlotWidget::BottomAxis)->majorTickMarks();
//    m_plot->setLimits(floor(*xp1), ceil(*xp2), floor(*yp1), ceil(*yp2));
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
