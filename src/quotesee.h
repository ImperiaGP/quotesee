/*   QuoteSee 0.2.3
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

#ifndef QUOTESEE_H
#define QUOTESEE_H
/********************************************/
#include </usr/include/KDE/Plasma/Applet>
#include </usr/include/KDE/KIcon>
#include </usr/include/KDE/KConfigDialog>
#include </usr/include/KDE/Plasma/DataEngine>
#include </usr/include/KDE/KUrl>
/********************************************/
#include <QtGui/QtGui>
#include <QtCore/QXmlStreamReader>
#include <QtNetwork/QHttpResponseHeader>
#include <QtNetwork/QtNetwork>
#include <QtNetwork/QHttp>
#include <QtCore/QUrl>
#include "quote.h"
#include "ui_qsconfig.h"
#include "ui_qsgeneralconfig.h"

class QuoteSee : public Plasma::Applet
{
    Q_OBJECT
public:
    QuoteSee(QObject *parent, const QVariantList &args);
    ~QuoteSee();
    void            paintInterface(QPainter *painter,
                    const QStyleOptionGraphicsItem *option,
                    const QRect& contentsRect);
    void            init();
    void            createConfigurationInterface(KConfigDialog *parent);

private:
    /**
     * @short holds list of Quote items, useful for displaying items in user entered order
     */
    typedef         QList<QGraphicsWidget *> WidgetList;

    /**
     * @short icon for the widget
     */
    KIcon           m_icon;

    Ui::QSConfig    m_config_ui;
    Ui::QSGeneralConfig m_general_config_ui;

    QStringList     m_code_list;
    WidgetList      m_quote_list;

    QGraphicsLinearLayout   *m_layout;

    QFileInfo       *fileLocation;

    Plasma::DataEngine      *m_engine;
    int             m_polling_minutes;
    int             m_polling_interval;

    bool            m_translucent;

    void            cleanLayout(QGraphicsLinearLayout*);
    void            createLayout();
    void            paintQuotes();

    void            connectSources();
    void            disconnectSources();

    void            updateSize();

    void            readConfig();
    void            saveConfig();

private slots:
    void            createConfig();
    void            loadFile();
    void            saveFile();
    void            addSource(const QString&);

protected:
    void            constraintsEvent(Plasma::Constraints constraints);
    void            resizeEvent(QGraphicsSceneResizeEvent *event);
//    void            debugOutput(QtMsgType type, const char *msg);
};

K_EXPORT_PLASMA_APPLET(quotesee, QuoteSee)
#endif // QUOTESEE_H
