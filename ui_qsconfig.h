/********************************************************************************
** Form generated from reading ui file 'qsconfig.ui'
**
** Created: Wed Apr 14 21:58:02 2010
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_QSCONFIG_H
#define UI_QSCONFIG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "keditlistbox.h"
#include "kseparator.h"
#include "kurlrequester.h"

QT_BEGIN_NAMESPACE

class Ui_QSConfig
{
public:
    QVBoxLayout *verticalLayout_3;
    QGroupBox *fromFileGroupBox;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    KUrlRequester *ieCodesRequester;
    QPushButton *loadButton;
    QPushButton *saveButton;
    KSeparator *kseparator_2;
    QGroupBox *manageSymbolsGroupBox;
    QVBoxLayout *verticalLayout_4;
    KEditListBox *quoteCodeList;

    void setupUi(QWidget *QSConfig)
    {
        if (QSConfig->objectName().isEmpty())
            QSConfig->setObjectName(QString::fromUtf8("QSConfig"));
        QSConfig->resize(496, 500);
        verticalLayout_3 = new QVBoxLayout(QSConfig);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        fromFileGroupBox = new QGroupBox(QSConfig);
        fromFileGroupBox->setObjectName(QString::fromUtf8("fromFileGroupBox"));
        verticalLayout = new QVBoxLayout(fromFileGroupBox);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        ieCodesRequester = new KUrlRequester(fromFileGroupBox);
        ieCodesRequester->setObjectName(QString::fromUtf8("ieCodesRequester"));

        horizontalLayout->addWidget(ieCodesRequester);

        loadButton = new QPushButton(fromFileGroupBox);
        loadButton->setObjectName(QString::fromUtf8("loadButton"));

        horizontalLayout->addWidget(loadButton);

        saveButton = new QPushButton(fromFileGroupBox);
        saveButton->setObjectName(QString::fromUtf8("saveButton"));

        horizontalLayout->addWidget(saveButton);


        verticalLayout->addLayout(horizontalLayout);


        verticalLayout_3->addWidget(fromFileGroupBox);

        kseparator_2 = new KSeparator(QSConfig);
        kseparator_2->setObjectName(QString::fromUtf8("kseparator_2"));

        verticalLayout_3->addWidget(kseparator_2);

        manageSymbolsGroupBox = new QGroupBox(QSConfig);
        manageSymbolsGroupBox->setObjectName(QString::fromUtf8("manageSymbolsGroupBox"));
        verticalLayout_4 = new QVBoxLayout(manageSymbolsGroupBox);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        quoteCodeList = new KEditListBox(manageSymbolsGroupBox);
        quoteCodeList->setObjectName(QString::fromUtf8("quoteCodeList"));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(quoteCodeList->sizePolicy().hasHeightForWidth());
        quoteCodeList->setSizePolicy(sizePolicy);

        verticalLayout_4->addWidget(quoteCodeList);


        verticalLayout_3->addWidget(manageSymbolsGroupBox);


        retranslateUi(QSConfig);

        QMetaObject::connectSlotsByName(QSConfig);
    } // setupUi

    void retranslateUi(QWidget *QSConfig)
    {
        QSConfig->setWindowTitle(QApplication::translate("QSConfig", "Form", 0, QApplication::UnicodeUTF8));
        fromFileGroupBox->setTitle(QApplication::translate("QSConfig", "Load symbol list from or save to file", 0, QApplication::UnicodeUTF8));
        loadButton->setText(QApplication::translate("QSConfig", "Load", 0, QApplication::UnicodeUTF8));
        saveButton->setText(QApplication::translate("QSConfig", "Save", 0, QApplication::UnicodeUTF8));
        manageSymbolsGroupBox->setTitle(QApplication::translate("QSConfig", "Manage symbols", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(QSConfig);
    } // retranslateUi

};

namespace Ui {
    class QSConfig: public Ui_QSConfig {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QSCONFIG_H
