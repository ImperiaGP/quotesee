#ifndef QSCONFIG_H
#define QSCONFIG_H

#include <QtGui/QWidget>

namespace Ui {
    class QSConfig;
}

class QSConfig : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(QSConfig)
public:
    explicit QSConfig(QWidget *parent = 0);
    virtual ~QSConfig();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::QSConfig *m_ui;

};

#endif // QSCONFIG_H
