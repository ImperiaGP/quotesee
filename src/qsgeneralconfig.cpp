#include "qsgeneralconfig.h"
#include "ui_qsgeneralconfig.h"

QSGeneralConfig::QSGeneralConfig(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::QSGeneralConfig)
{
    m_ui->setupUi(this);
}

QSGeneralConfig::~QSGeneralConfig()
{
    delete m_ui;
}

void QSGeneralConfig::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
