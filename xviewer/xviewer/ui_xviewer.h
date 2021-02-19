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
    QWidget *body;
    QWidget *left;
    QWidget *cams;

    void setupUi(QWidget *XViewerClass)
    {
        if (XViewerClass->objectName().isEmpty())
            XViewerClass->setObjectName(QString::fromUtf8("XViewerClass"));
        XViewerClass->resize(800, 600);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/XViewer/img/xv.ico"), QSize(), QIcon::Normal, QIcon::Off);
        XViewerClass->setWindowIcon(icon);
        XViewerClass->setStyleSheet(QString::fromUtf8("/* \351\241\266\351\203\250\346\240\267\345\274\217 */\n"
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
"background-image: url(:/XViewer/img/close.png);\n"
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
        head_button->setGeometry(QRect(690, 10, 101, 31));
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
        body = new QWidget(XViewerClass);
        body->setObjectName(QString::fromUtf8("body"));
        body->setGeometry(QRect(0, 50, 801, 541));
        left = new QWidget(body);
        left->setObjectName(QString::fromUtf8("left"));
        left->setGeometry(QRect(9, 19, 200, 591));
        left->setMaximumSize(QSize(200, 16777215));
        cams = new QWidget(body);
        cams->setObjectName(QString::fromUtf8("cams"));
        cams->setGeometry(QRect(189, 19, 571, 501));

        retranslateUi(XViewerClass);
        QObject::connect(close, SIGNAL(clicked()), XViewerClass, SLOT(close()));
        QObject::connect(normal, SIGNAL(clicked()), XViewerClass, SLOT(NormalWindow()));
        QObject::connect(min, SIGNAL(clicked()), XViewerClass, SLOT(showMinimized()));
        QObject::connect(max, SIGNAL(clicked()), XViewerClass, SLOT(MaxWindow()));

        QMetaObject::connectSlotsByName(XViewerClass);
    } // setupUi

    void retranslateUi(QWidget *XViewerClass)
    {
        XViewerClass->setWindowTitle(QCoreApplication::translate("XViewerClass", "XViewer", nullptr));
        min->setText(QString());
        max->setText(QString());
        close->setText(QString());
        normal->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class XViewerClass: public Ui_XViewerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_XVIEWER_H
