/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.10
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QGroupBox *group_display;
    QGridLayout *gridLayout_3;
    QGridLayout *gridLayout_2;
    QSpacerItem *horizontalSpacer_7;
    QLabel *label_8;
    QSpacerItem *horizontalSpacer_3;
    QLabel *label_AoLP;
    QLabel *label_12;
    QLabel *label_DoLP;
    QSpacerItem *horizontalSpacer_4;
    QSpacerItem *horizontalSpacer_6;
    QLabel *label_RGB;
    QLabel *label_Depth;
    QLabel *label_11;
    QLabel *label_7;
    QLabel *label_9;
    QLabel *label_Intensity;
    QLabel *label_10;
    QSpacerItem *horizontalSpacer_5;
    QLabel *label_Normal;
    QSpacerItem *horizontalSpacer_8;
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_2;
    QGroupBox *group_control;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *btn_processing;
    QSpacerItem *horizontalSpacer;
    QPushButton *btn_freeze;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *btn_capture;
    QMenuBar *menubar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(2061, 1270);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        group_display = new QGroupBox(centralwidget);
        group_display->setObjectName(QString::fromUtf8("group_display"));
        group_display->setMinimumSize(QSize(0, 0));
        gridLayout_3 = new QGridLayout(group_display);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        horizontalSpacer_7 = new QSpacerItem(10, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_7, 1, 0, 1, 1);

        label_8 = new QLabel(group_display);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_8, 0, 1, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(10, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_3, 1, 2, 1, 1);

        label_AoLP = new QLabel(group_display);
        label_AoLP->setObjectName(QString::fromUtf8("label_AoLP"));
        label_AoLP->setMinimumSize(QSize(640, 480));

        gridLayout_2->addWidget(label_AoLP, 1, 5, 1, 1);

        label_12 = new QLabel(group_display);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_12, 0, 5, 1, 1);

        label_DoLP = new QLabel(group_display);
        label_DoLP->setObjectName(QString::fromUtf8("label_DoLP"));
        label_DoLP->setMinimumSize(QSize(640, 480));

        gridLayout_2->addWidget(label_DoLP, 1, 3, 1, 1);

        horizontalSpacer_4 = new QSpacerItem(10, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_4, 1, 4, 1, 1);

        horizontalSpacer_6 = new QSpacerItem(10, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_6, 3, 4, 1, 1);

        label_RGB = new QLabel(group_display);
        label_RGB->setObjectName(QString::fromUtf8("label_RGB"));
        label_RGB->setMinimumSize(QSize(640, 480));

        gridLayout_2->addWidget(label_RGB, 3, 1, 1, 1);

        label_Depth = new QLabel(group_display);
        label_Depth->setObjectName(QString::fromUtf8("label_Depth"));
        label_Depth->setMinimumSize(QSize(640, 480));

        gridLayout_2->addWidget(label_Depth, 3, 3, 1, 1);

        label_11 = new QLabel(group_display);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_11, 0, 3, 1, 1);

        label_7 = new QLabel(group_display);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_7, 2, 1, 1, 1);

        label_9 = new QLabel(group_display);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setTextFormat(Qt::RichText);
        label_9->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_9, 2, 3, 1, 1);

        label_Intensity = new QLabel(group_display);
        label_Intensity->setObjectName(QString::fromUtf8("label_Intensity"));
        label_Intensity->setMinimumSize(QSize(640, 480));

        gridLayout_2->addWidget(label_Intensity, 1, 1, 1, 1);

        label_10 = new QLabel(group_display);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_10, 2, 5, 1, 1);

        horizontalSpacer_5 = new QSpacerItem(10, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_5, 3, 2, 1, 1);

        label_Normal = new QLabel(group_display);
        label_Normal->setObjectName(QString::fromUtf8("label_Normal"));
        label_Normal->setMinimumSize(QSize(640, 480));

        gridLayout_2->addWidget(label_Normal, 3, 5, 1, 1);

        horizontalSpacer_8 = new QSpacerItem(10, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_8, 1, 6, 1, 1);


        gridLayout_3->addLayout(gridLayout_2, 0, 0, 1, 1);


        verticalLayout->addWidget(group_display);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);


        verticalLayout->addLayout(verticalLayout_2);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        group_control = new QGroupBox(centralwidget);
        group_control->setObjectName(QString::fromUtf8("group_control"));
        horizontalLayout_3 = new QHBoxLayout(group_control);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        btn_processing = new QPushButton(group_control);
        btn_processing->setObjectName(QString::fromUtf8("btn_processing"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(btn_processing->sizePolicy().hasHeightForWidth());
        btn_processing->setSizePolicy(sizePolicy);
        btn_processing->setMinimumSize(QSize(300, 0));

        horizontalLayout_3->addWidget(btn_processing);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        btn_freeze = new QPushButton(group_control);
        btn_freeze->setObjectName(QString::fromUtf8("btn_freeze"));
        sizePolicy.setHeightForWidth(btn_freeze->sizePolicy().hasHeightForWidth());
        btn_freeze->setSizePolicy(sizePolicy);
        btn_freeze->setMinimumSize(QSize(300, 0));

        horizontalLayout_3->addWidget(btn_freeze);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);

        btn_capture = new QPushButton(group_control);
        btn_capture->setObjectName(QString::fromUtf8("btn_capture"));
        sizePolicy.setHeightForWidth(btn_capture->sizePolicy().hasHeightForWidth());
        btn_capture->setSizePolicy(sizePolicy);
        btn_capture->setMinimumSize(QSize(300, 100));

        horizontalLayout_3->addWidget(btn_capture);


        horizontalLayout_2->addWidget(group_control);


        verticalLayout->addLayout(horizontalLayout_2);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 2061, 23));
        MainWindow->setMenuBar(menubar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        group_display->setTitle(QApplication::translate("MainWindow", "Display", nullptr));
        label_8->setText(QApplication::translate("MainWindow", "Intensity", nullptr));
        label_AoLP->setText(QApplication::translate("MainWindow", "TextLabel", nullptr));
        label_12->setText(QApplication::translate("MainWindow", "AoLP", nullptr));
        label_DoLP->setText(QApplication::translate("MainWindow", "TextLabel", nullptr));
        label_RGB->setText(QApplication::translate("MainWindow", "TextLabel", nullptr));
        label_Depth->setText(QApplication::translate("MainWindow", "TextLabel", nullptr));
        label_11->setText(QApplication::translate("MainWindow", "DoLP", nullptr));
        label_7->setText(QApplication::translate("MainWindow", "RGB", nullptr));
        label_9->setText(QApplication::translate("MainWindow", "Depth", nullptr));
        label_Intensity->setText(QApplication::translate("MainWindow", "TextLabel", nullptr));
        label_10->setText(QApplication::translate("MainWindow", "Normal", nullptr));
        label_Normal->setText(QApplication::translate("MainWindow", "TextLabel", nullptr));
        group_control->setTitle(QApplication::translate("MainWindow", "Control", nullptr));
        btn_processing->setText(QApplication::translate("MainWindow", "Processing", nullptr));
        btn_freeze->setText(QApplication::translate("MainWindow", "Freeze", nullptr));
        btn_capture->setText(QApplication::translate("MainWindow", "Capture", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
