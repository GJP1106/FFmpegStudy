/********************************************************************************
** Form generated from reading UI file 'xviewer.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_XVIEWER_H
#define UI_XVIEWER_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_XViewerClass
{
public:
    QWidget *head;
    QWidget *logo;
    QWidget *head_button;
    QPushButton *min;
    QPushButton *max;
    QPushButton *close;
    QPushButton *normal;
    QLabel *status;
    QWidget *body;
    QWidget *left;
    QListWidget *cam_list;
    QPushButton *add_cam;
    QPushButton *set_cam;
    QPushButton *del_cam;
    QWidget *cams;

    void setupUi(QWidget *XViewerClass)
    {
        if (XViewerClass->objectName().isEmpty())
            XViewerClass->setObjectName(QString::fromUtf8("XViewerClass"));
        XViewerClass->resize(800, 600);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/XViewer/img/xv.ico"), QSize(), QIcon::Normal, QIcon::Off);
        XViewerClass->setWindowIcon(icon);
        XViewerClass->setStyleSheet(QString::fromUtf8("/* \346\214\211\351\222\256\346\240\267\345\274\217 */\n"
"QPushButton:hover\n"
"\n"
" { \n"
"background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(0, 102, 184, 255), stop:0.495 rgba(39, 39, 39, 255), stop:0.505 rgba(39,39, 39, 255), stop:1 rgba(0, 102, 184, 255));\n"
"border: none;\n"
"border-radius:5px;\n"
"color: rgb(255, 255, 255);\n"
"font: 75 12pt \"\351\273\221\344\275\223\";\n"
" }\n"
"QPushButton:!hover\n"
" { \n"
"	background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(0, 50, 150, 255), stop:0.495 rgba(0, 102, 184, 255), stop:0.505 rgba(0, 102, 184, 255), stop:1 rgba(0, 50, 150, 255));\n"
" border: none;\n"
"border-radius:5px;\n"
"color: rgb(255, 255, 255);\n"
"font: 75 12pt \"\351\273\221\344\275\223\";\n"
" }\n"
"/* \351\241\266\351\203\250\346\240\267\345\274\217 */\n"
"#head\n"
"{\n"
"background-color:#3c3c3c;\n"
"}\n"
"#logo\n"
"{\n"
"background-image: url(:/XViewer/img/logo_150_40.png);\n"
"}\n"
"#close\n"
"{\n"
"background-image: url(:/XVi"
                        "ewer/img/close.png);\n"
"}\n"
"#min\n"
"{\n"
"background-image: url(:/XViewer/img/min.png);\n"
"}\n"
"#max\n"
"{\n"
"background-image: url(:/XViewer/img/max.png);\n"
"}\n"
"#normal\n"
"{\n"
"background-image: url(:/XViewer/img/normal.png);\n"
"}\n"
"/* body \346\240\267\345\274\217*/\n"
"#body\n"
"{\n"
"background-color:#212121;\n"
"}\n"
"#left\n"
"{\n"
"background-color:#252525;\n"
"}\n"
"#cams\n"
"{\n"
"background-color:#1e1e1e;\n"
"}\n"
"#status\n"
"{\n"
"color: rgb(255, 255, 255);\n"
"	font: 75 10pt \"Adobe Devanagari\";\n"
"}"));
        head = new QWidget(XViewerClass);
        head->setObjectName(QString::fromUtf8("head"));
        head->setGeometry(QRect(0, 0, 800, 50));
        head->setMaximumSize(QSize(16777215, 50));
        head->setStyleSheet(QString::fromUtf8(""));
        logo = new QWidget(head);
        logo->setObjectName(QString::fromUtf8("logo"));
        logo->setGeometry(QRect(0, 5, 151, 41));
        head_button = new QWidget(head);
        head_button->setObjectName(QString::fromUtf8("head_button"));
        head_button->setGeometry(QRect(690, 10, 91, 41));
        min = new QPushButton(head_button);
        min->setObjectName(QString::fromUtf8("min"));
        min->setGeometry(QRect(10, 0, 20, 20));
        min->setFlat(true);
        max = new QPushButton(head_button);
        max->setObjectName(QString::fromUtf8("max"));
        max->setGeometry(QRect(40, 0, 20, 20));
        max->setFlat(true);
        close = new QPushButton(head_button);
        close->setObjectName(QString::fromUtf8("close"));
        close->setGeometry(QRect(70, 0, 20, 20));
        close->setFlat(true);
        normal = new QPushButton(head_button);
        normal->setObjectName(QString::fromUtf8("normal"));
        normal->setGeometry(QRect(40, 0, 20, 20));
        normal->setFlat(true);
        min->raise();
        close->raise();
        normal->raise();
        max->raise();
        status = new QLabel(head);
        status->setObjectName(QString::fromUtf8("status"));
        status->setGeometry(QRect(240, 15, 191, 31));
        body = new QWidget(XViewerClass);
        body->setObjectName(QString::fromUtf8("body"));
        body->setGeometry(QRect(0, 50, 701, 371));
        left = new QWidget(body);
        left->setObjectName(QString::fromUtf8("left"));
        left->setGeometry(QRect(10, 10, 200, 600));
        left->setMaximumSize(QSize(200, 16777215));
        cam_list = new QListWidget(left);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/XViewer/img/cam.png"), QSize(), QIcon::Normal, QIcon::Off);
        QListWidgetItem *__qlistwidgetitem = new QListWidgetItem(cam_list);
        __qlistwidgetitem->setIcon(icon1);
        cam_list->setObjectName(QString::fromUtf8("cam_list"));
        cam_list->setGeometry(QRect(0, 39, 201, 961));
        cam_list->setDragEnabled(true);
        cam_list->setIconSize(QSize(30, 30));
        add_cam = new QPushButton(left);
        add_cam->setObjectName(QString::fromUtf8("add_cam"));
        add_cam->setGeometry(QRect(5, 5, 51, 31));
        set_cam = new QPushButton(left);
        set_cam->setObjectName(QString::fromUtf8("set_cam"));
        set_cam->setGeometry(QRect(70, 5, 51, 31));
        del_cam = new QPushButton(left);
        del_cam->setObjectName(QString::fromUtf8("del_cam"));
        del_cam->setGeometry(QRect(140, 5, 51, 31));
        cams = new QWidget(body);
        cams->setObjectName(QString::fromUtf8("cams"));
        cams->setGeometry(QRect(220, 10, 501, 351));

        retranslateUi(XViewerClass);
        QObject::connect(close, SIGNAL(clicked()), XViewerClass, SLOT(close()));
        QObject::connect(normal, SIGNAL(clicked()), XViewerClass, SLOT(NormalWindow()));
        QObject::connect(min, SIGNAL(clicked()), XViewerClass, SLOT(showMinimized()));
        QObject::connect(max, SIGNAL(clicked()), XViewerClass, SLOT(MaxWindow()));
        QObject::connect(add_cam, SIGNAL(clicked()), XViewerClass, SLOT(AddCam()));
        QObject::connect(set_cam, SIGNAL(clicked()), XViewerClass, SLOT(SetCam()));
        QObject::connect(del_cam, SIGNAL(clicked()), XViewerClass, SLOT(DelCam()));

        QMetaObject::connectSlotsByName(XViewerClass);
    } // setupUi

    void retranslateUi(QWidget *XViewerClass)
    {
        XViewerClass->setWindowTitle(QCoreApplication::translate("XViewerClass", "XViewer", nullptr));
        min->setText(QString());
        max->setText(QString());
        close->setText(QString());
        normal->setText(QString());
        status->setText(QCoreApplication::translate("XViewerClass", "\347\233\221\346\216\247\344\270\255....", nullptr));

        const bool __sortingEnabled = cam_list->isSortingEnabled();
        cam_list->setSortingEnabled(false);
        QListWidgetItem *___qlistwidgetitem = cam_list->item(0);
        ___qlistwidgetitem->setText(QCoreApplication::translate("XViewerClass", "camear_test", nullptr));
        cam_list->setSortingEnabled(__sortingEnabled);

        add_cam->setText(QCoreApplication::translate("XViewerClass", "\346\226\260\345\242\236", nullptr));
        set_cam->setText(QCoreApplication::translate("XViewerClass", "\344\277\256\346\224\271", nullptr));
        del_cam->setText(QCoreApplication::translate("XViewerClass", "\345\210\240\351\231\244", nullptr));
    } // retranslateUi

};

namespace Ui {
    class XViewerClass: public Ui_XViewerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_XVIEWER_H
