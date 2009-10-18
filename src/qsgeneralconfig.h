#ifndef QSGENERALCONFIG_H
#define QSGENERALCONFIG_H

#include <QtGui/QWidget>

namespace Ui {
    class QSGeneralConfig;
}

class QSGeneralConfig : public QWidget {
    Q_OBJECT
public:
    QSGeneralConfig(QWidget *parent = 0);
    ~QSGeneralConfig();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::QSGeneralConfig *m_ui;
};

#endif // QSGENERALCONFIG_H
