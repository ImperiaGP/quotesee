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

#include "quotesee.h"
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#define ___FILE___ ((strrchr(__FILE__, '/') ? : __FILE__ - 1) + 1)
#if !defined(QT_NO_DEBUG_OUTPUT)
#include "qsdebug.h"
#endif

using namespace::std;

#define UNIT_INTERVAL   60*1000
#define MINUTE_INTERVAL 1
#define MARGIN          14

QuoteSee::QuoteSee(QObject *parent, const QVariantList &args) :
    Plasma::Applet(parent, args),
    m_icon("quotesee"),
    m_code_list(0),
    m_quote_list(),
    m_layout(0),
    m_engine(0),
    m_polling_minutes(MINUTE_INTERVAL),
    m_polling_interval(UNIT_INTERVAL),
    m_translucent(true)
{
//#if !defined(NO_DEBUG_TO_FILE)
//    qInstallMsgHandler(qsDebug::debugOutput);
//#endif

    qDebug("\n\n-------------------\nQuoteSee instance start\n-------------------");

    qDebug() << "size on start: " << this->size().width() << this->size().height();
    setHasConfigurationInterface(true);

    setAspectRatioMode(Plasma::IgnoreAspectRatio);

    setMinimumSize(270, 100);
    setPreferredSize(270, 100);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
}

void QuoteSee::readConfig()
{
    KConfigGroup configGroup = config();
    m_code_list = configGroup.readEntry("quotes", QStringList());

    // if entry doesn't exist, it reads in 0, we don't want that
    if(!(configGroup.readEntry("polling_interval_minutes", int()) < 1))
    {
        m_polling_minutes = configGroup.readEntry("polling_interval_minutes", int());
    }

    m_translucent = configGroup.readEntry("translucent_background", bool());
}

void QuoteSee::saveConfig()
{
    KConfigGroup configGroup = config();
    configGroup.writeEntry("quotes", m_code_list);
    configGroup.writeEntry("polling_interval_minutes", m_polling_minutes);
    configGroup.writeEntry("translucent_background", m_translucent);
}

QuoteSee::~QuoteSee()
{
    if (hasFailedToLaunch())
    {
        kWarning() << "QuoteSee has failed to launch";
    }
    delete fileLocation;
    qDebug("\n\n-------------------\nQuoteSee instance end\n-------------------");
}

void QuoteSee::init()
{
    fileLocation = new QFileInfo;

    readConfig();

    m_engine = dataEngine("quotesee");
    connect(m_engine, SIGNAL(sourceAdded(QString)), this, SLOT(addSource(QString)));

    if (!m_engine->isValid())
    {
        setFailedToLaunch(true, "Error loading data engine");
    }

    if (m_icon.isNull())
    {
        setFailedToLaunch(true, "QuoteSee failed to launch...");
    }

    m_layout = new QGraphicsLinearLayout(Qt::Vertical, this);
    m_layout->setSpacing(5);
    m_layout->addStretch();

    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    if(!m_code_list.isEmpty())
        paintQuotes();
    else
        setConfigurationRequired(true, "Enter some ticker symbols");
}

void QuoteSee::addSource(const QString& source)
{
    Q_UNUSED(source);
    Plasma::DataEngine::Data data = m_engine->query(source);
}

void QuoteSee::constraintsEvent(Plasma::Constraints constraints)
{
//    if (constraints & Plasma::FormFactorConstraint || constraints & Plasma::SizeConstraint )
//    {
//        setBackgroundHints(m_translucent == true ? Plasma::Applet::TranslucentBackground
//                                                 : Plasma::Applet::DefaultBackground);

////        qreal left = MARGIN, top = MARGIN, right = MARGIN, bottom = MARGIN;
////        setContentsMargins(left, top, right, bottom);

//        // this isn't great, research other ways to adjust size
//        setContentsMargins(MARGIN, MARGIN, MARGIN, MARGIN);

        resize(m_layout->sizeHint(Qt::PreferredSize) + QSizeF(MARGIN+MARGIN, MARGIN+MARGIN));
//    }

//    if(layout()->count() == 1)
//    {
//        setContentsMargins(0.0, 0.0, 0.0, 0.0);
//    }
}

void QuoteSee::connectSources()
{
    foreach(QString code, m_code_list)
    {
        Quote* quote = new Quote(this);

        m_engine->connectSource(code.toLower(), quote, 1000 /*m_polling_interval * m_polling_minutes*/);

        m_quote_list.append(quote);
    }
}

void QuoteSee::disconnectSources()
{
    foreach(QGraphicsWidget *q, m_quote_list)
    {
        m_engine->disconnectSource(dynamic_cast<Quote *>(q)->getName(), q);
    }
}

void QuoteSee::paintInterface(QPainter *p, const QStyleOptionGraphicsItem *option, const QRect &contentsRect)
{
    Q_UNUSED(option);
    Q_UNUSED(contentsRect);
    p->save();

    p->setRenderHint(QPainter::SmoothPixmapTransform);
    p->setRenderHint(QPainter::Antialiasing);

    p->restore();
}

void QuoteSee::createConfigurationInterface(KConfigDialog *parent)
{
    QWidget *configInterface = new QWidget;

    m_config_ui.setupUi(configInterface);
    m_config_ui.quoteCodeList->setItems(m_code_list);
// USE setURL instead of setPath http://api.kde.org/4.4-api/kdelibs-apidocs/kio/html/classKUrlRequester.html#a62b31e599dc2626a94b43f38147bf6f3
    m_config_ui.ieCodesRequester->setUrl(KUrl(fileLocation->absoluteFilePath()));
//    m_config_ui.ieCodesRequester->setPath(fileLocation->absoluteFilePath());

    m_config_ui.ieCodesRequester->setMode(KFile::File | KFile::LocalOnly | KFile::ExistingOnly);
    m_config_ui.ieCodesRequester->setMode(KFile::File | KFile::LocalOnly);

    connect(m_config_ui.loadButton, SIGNAL(clicked()), this, SLOT(loadFile()));
    connect(m_config_ui.saveButton, SIGNAL(clicked()), this, SLOT(saveFile()));

    parent->addPage(configInterface, "Symbol settings", icon(), QString(), false);

    QWidget *generalConfigInterface = new QWidget;
    m_general_config_ui.setupUi(generalConfigInterface);

    m_general_config_ui.updateIntervalSpinBox->setValue(m_polling_minutes);
    m_general_config_ui.translucentCheckBox->setChecked(m_translucent);

    parent->addPage(generalConfigInterface, "General settings", icon(), QString(), false);

    connect(parent, SIGNAL(okClicked()), this, SLOT(createConfig()));
}

void QuoteSee::createConfig()
{
    fileLocation->setFile(m_config_ui.ieCodesRequester->url().toLocalFile());

    if((m_code_list != m_config_ui.quoteCodeList->items()) ||
       (m_polling_minutes != m_general_config_ui.updateIntervalSpinBox->value()) ||
       (m_translucent != m_general_config_ui.translucentCheckBox->isChecked()))
    {
        m_code_list = m_config_ui.quoteCodeList->items();
        m_polling_minutes = m_general_config_ui.updateIntervalSpinBox->value();
        m_translucent = m_general_config_ui.translucentCheckBox->isChecked();

        !m_code_list.isEmpty() ? setConfigurationRequired(false)
                               : setConfigurationRequired(true);

        saveConfig();
        paintQuotes();
    }

    emit configNeedsSaving();
}

void QuoteSee::paintQuotes()
{
    prepareGeometryChange();

    disconnectSources();
    cleanLayout(m_layout);
    m_quote_list.clear();
    connectSources();

    foreach(QGraphicsWidget *q, m_quote_list)
    {
        if(q->isWidget())
            m_layout->insertItem(m_layout->count() - 1, q);
    }

    updateConstraints();
}

void QuoteSee::cleanLayout(QGraphicsLinearLayout* layout)
{
    for (int i = layout->count() - 1; i >= 0; --i)
    {
        QGraphicsLayoutItem* item = layout->itemAt(i);

        layout->removeAt(i);
        delete item;
    }
}

void QuoteSee::loadFile()
{
    QFileInfo fileLoc(m_config_ui.ieCodesRequester->url().toLocalFile());

    if(fileLoc.isFile() && fileLoc.size() != 0)
    {
        QFile file(fileLoc.absoluteFilePath());

        m_config_ui.quoteCodeList->clear();

        if (!file.open(QIODevice::ReadOnly))
        {
            kWarning() << "Error: " << file.errorString();
            return;
        }

        QTextStream stream ( &file );
        QString line;

        while( !stream.atEnd()) {
             m_config_ui.quoteCodeList->insertItem(stream.readLine());
        }
        file.close();
    }
    else
        return;
}

void QuoteSee::saveFile()
{
    QFileInfo fileLoc(m_config_ui.ieCodesRequester->url().toLocalFile());

    QFile file(fileLoc.absoluteFilePath());

     if (file.open(QFile::WriteOnly | QFile::Truncate))
    {
        QTextStream out(&file);

        foreach(QString code, m_config_ui.quoteCodeList->items())
            out << code << "\r\n";
    }
     else
     {
        qWarning() << "Error: " << file.errorString();
        return;
    }
    file.close();
}

#include "quotesee.moc"
