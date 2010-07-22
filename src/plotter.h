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
    void setLimits( double x1, double x2, double y1, double y2 );
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

    Plasma::Svg *m_item_background;
signals:

public slots:

};

#endif // PLOTTER_H
