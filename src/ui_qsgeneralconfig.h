/********************************************************************************
** Form generated from reading ui file 'qsgeneralconfig.ui'
**
** Created: Mon Aug 10 14:18:39 2009
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_QSGENERALCONFIG_H
#define UI_QSGENERALCONFIG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "kseparator.h"

QT_BEGIN_NAMESPACE

class Ui_QSGeneralConfig
{
public:
    QVBoxLayout *verticalLayout_3;
    QGroupBox *appearanceGroupBox;
    QVBoxLayout *verticalLayout;
    QCheckBox *translucentCheckBox;
    KSeparator *kseparator;
    QGroupBox *intervalGroupBox;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QSpinBox *updateIntervalSpinBox;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *QSGeneralConfig)
    {
        if (QSGeneralConfig->objectName().isEmpty())
            QSGeneralConfig->setObjectName(QString::fromUtf8("QSGeneralConfig"));
        QSGeneralConfig->resize(400, 300);
        verticalLayout_3 = new QVBoxLayout(QSGeneralConfig);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        appearanceGroupBox = new QGroupBox(QSGeneralConfig);
        appearanceGroupBox->setObjectName(QString::fromUtf8("appearanceGroupBox"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(appearanceGroupBox->sizePolicy().hasHeightForWidth());
        appearanceGroupBox->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(appearanceGroupBox);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        translucentCheckBox = new QCheckBox(appearanceGroupBox);
        translucentCheckBox->setObjectName(QString::fromUtf8("translucentCheckBox"));
        translucentCheckBox->setChecked(true);

        verticalLayout->addWidget(translucentCheckBox);


        verticalLayout_3->addWidget(appearanceGroupBox);

        kseparator = new KSeparator(QSGeneralConfig);
        kseparator->setObjectName(QString::fromUtf8("kseparator"));

        verticalLayout_3->addWidget(kseparator);

        intervalGroupBox = new QGroupBox(QSGeneralConfig);
        intervalGroupBox->setObjectName(QString::fromUtf8("intervalGroupBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(intervalGroupBox->sizePolicy().hasHeightForWidth());
        intervalGroupBox->setSizePolicy(sizePolicy1);
        verticalLayout_2 = new QVBoxLayout(intervalGroupBox);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_3 = new QLabel(intervalGroupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        sizePolicy.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy);

        horizontalLayout_3->addWidget(label_3);

        updateIntervalSpinBox = new QSpinBox(intervalGroupBox);
        updateIntervalSpinBox->setObjectName(QString::fromUtf8("updateIntervalSpinBox"));
        QSizePolicy sizePolicy2(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(updateIntervalSpinBox->sizePolicy().hasHeightForWidth());
        updateIntervalSpinBox->setSizePolicy(sizePolicy2);
        updateIntervalSpinBox->setMinimum(1);
        updateIntervalSpinBox->setMaximum(120);
        updateIntervalSpinBox->setValue(3);

        horizontalLayout_3->addWidget(updateIntervalSpinBox);


        verticalLayout_2->addLayout(horizontalLayout_3);


        verticalLayout_3->addWidget(intervalGroupBox);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer);


        retranslateUi(QSGeneralConfig);

        QMetaObject::connectSlotsByName(QSGeneralConfig);
    } // setupUi

    void retranslateUi(QWidget *QSGeneralConfig)
    {
        QSGeneralConfig->setWindowTitle(QApplication::translate("QSGeneralConfig", "Form", 0, QApplication::UnicodeUTF8));
        appearanceGroupBox->setTitle(QApplication::translate("QSGeneralConfig", "Appearance", 0, QApplication::UnicodeUTF8));
        translucentCheckBox->setText(QApplication::translate("QSGeneralConfig", "Make QuoteSee translucent", 0, QApplication::UnicodeUTF8));
        intervalGroupBox->setTitle(QApplication::translate("QSGeneralConfig", "Adjust update interval", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("QSGeneralConfig", "Update Interval [min]", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(QSGeneralConfig);
    } // retranslateUi

};

namespace Ui {
    class QSGeneralConfig: public Ui_QSGeneralConfig {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QSGENERALCONFIG_H
