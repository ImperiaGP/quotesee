#include "qsconfig.h"
#include "ui_qsconfig.h"

QSConfig::QSConfig(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::QSConfig)
{
    m_ui->setupUi(this);
}

QSConfig::~QSConfig()
{
    delete m_ui;
}

void QSConfig::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
