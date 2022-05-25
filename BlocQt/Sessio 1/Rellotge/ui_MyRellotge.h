/********************************************************************************
** Form generated from reading UI file 'MyRellotge.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MYRELLOTGE_H
#define UI_MYRELLOTGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDial>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MyRellotge
{
public:
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QLCDNumber *lcdNumber;
    QDial *dial;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_2;
    QLCDNumber *lcdNumber_2;
    QDial *dial_2;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton;

    void setupUi(QWidget *MyRellotge)
    {
        if (MyRellotge->objectName().isEmpty())
            MyRellotge->setObjectName(QString::fromUtf8("MyRellotge"));
        MyRellotge->resize(416, 342);
        verticalLayout_3 = new QVBoxLayout(MyRellotge);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(MyRellotge);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout->addWidget(label);

        lcdNumber = new QLCDNumber(MyRellotge);
        lcdNumber->setObjectName(QString::fromUtf8("lcdNumber"));

        verticalLayout->addWidget(lcdNumber);

        dial = new QDial(MyRellotge);
        dial->setObjectName(QString::fromUtf8("dial"));
        dial->setMaximum(23);

        verticalLayout->addWidget(dial);


        horizontalLayout_2->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        label_2 = new QLabel(MyRellotge);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        verticalLayout_2->addWidget(label_2);

        lcdNumber_2 = new QLCDNumber(MyRellotge);
        lcdNumber_2->setObjectName(QString::fromUtf8("lcdNumber_2"));

        verticalLayout_2->addWidget(lcdNumber_2);

        dial_2 = new QDial(MyRellotge);
        dial_2->setObjectName(QString::fromUtf8("dial_2"));
        dial_2->setMaximum(59);

        verticalLayout_2->addWidget(dial_2);


        horizontalLayout_2->addLayout(verticalLayout_2);


        verticalLayout_3->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButton = new QPushButton(MyRellotge);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        horizontalLayout->addWidget(pushButton);


        verticalLayout_3->addLayout(horizontalLayout);


        retranslateUi(MyRellotge);
        QObject::connect(dial, SIGNAL(valueChanged(int)), lcdNumber, SLOT(display(int)));
        QObject::connect(dial_2, SIGNAL(valueChanged(int)), lcdNumber_2, SLOT(display(int)));
        QObject::connect(pushButton, SIGNAL(clicked()), MyRellotge, SLOT(close()));

        QMetaObject::connectSlotsByName(MyRellotge);
    } // setupUi

    void retranslateUi(QWidget *MyRellotge)
    {
        MyRellotge->setWindowTitle(QApplication::translate("MyRellotge", "Form", nullptr));
        label->setText(QApplication::translate("MyRellotge", "Hours", nullptr));
        label_2->setText(QApplication::translate("MyRellotge", "Minutes", nullptr));
        pushButton->setText(QApplication::translate("MyRellotge", "Exit", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MyRellotge: public Ui_MyRellotge {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MYRELLOTGE_H
