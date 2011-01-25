#ifndef PLOTTER_H
#define PLOTTER_H

#include <QGraphicsWidget>

class QGraphicsProxyWidget;
class QGraphicsLinearLayout;
class KPlotObject;
class KPlotWidget;
class QLabel;

namespace Plasma
{
  class Svg;
}

class Plotter : public QGraphicsWidget
{
    Q_OBJECT
public:
    explicit Plotter(QGraphicsWidget *parent);

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    /**
     * Add an item to the list of KPlotObjects to be plotted - reimplemented from kplotwidget.
     * @note do not use this multiple time if many objects have to be added,
     * addPlotObjects() is strongly suggested in this case
     * @param object the KPlotObject to be added
     */
    void addPlotObject( KPlotObject *object );

    /**
     * Add more than one KPlotObject at one time - reimplemented from kplotwidget.
     * @param objects the list of KPlotObjects to be added
     */
    void addPlotObjects( const QList< KPlotObject* >& objects );

    void setName(QString n) {name = n;}
    void setInfo(QString i) {info = i; }
    void setLabelText(QString text);

    /**
     * Set x and y scale for axis
     * @param x1 axis origin on left
     * @param x2 axis end on right, adds one minute to make space for point inserted at time x2'
     * @param length length of the axis, calculated as x2 - length, expands if x1 < x2 - length
     * @param y1 lower limit of y axis
     * @param y2 upper limit of y axis
     */
    void setLimits( double x1, double x2, double length, double y1, double y2 );

    /**
     * Calculate origin of the axis give span and most recently inserted point
     * @param currentTime most recently inserted point
     * @param span length of the axis from @currentTime to past
     * @returns time at which to start the axis
     */
    double axisStart(double currentTime, int span);

    /**
     * Wrapper for KPlotWidget::plotObjects()
     */
    QList< KPlotObject* > plotObjects() const;
private:
    QGraphicsLinearLayout *m_layout;
    KPlotWidget *m_plot;

    /**
     * adds plot to widget
     */
    void    addPlot();

    /**
     * configures the look of plot
     */
    void    configPlot();

    /**
     * placeholder function, adds arbitrary points to graph
     */
    void    addPlotPoints();

    void addInfoLabel();

    QLabel *infoLabel;

    QString name;
    QString info;

    double toAxisTime(double val);
    QString timeToString(double t);
    Plasma::Svg *m_item_background;
signals:

public slots:

};

#endif // PLOTTER_H
