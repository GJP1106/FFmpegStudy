/********************************************************************************
** Form generated from reading UI file 'yuvrgbplayer.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_YUVRGBPLAYER_H
#define UI_YUVRGBPLAYER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_xvideoViewClass
{
public:
    QLabel *video1;
    QLabel *video2;
    QPushButton *open1;
    QComboBox *pix1;
    QSpinBox *width1;
    QSpinBox *height1;
    QSpinBox *set_fps1;
    QSpinBox *set_fps2;
    QSpinBox *height2;
    QComboBox *pix2;
    QSpinBox *width2;
    QPushButton *open2;
    QLabel *fps1;
    QLabel *fps2;

    void setupUi(QWidget *xvideoViewClass)
    {
        if (xvideoViewClass->objectName().isEmpty())
            xvideoViewClass->setObjectName(QString::fromUtf8("xvideoViewClass"));
        xvideoViewClass->resize(600, 400);
        video1 = new QLabel(xvideoViewClass);
        video1->setObjectName(QString::fromUtf8("video1"));
        video1->setGeometry(QRect(30, 20, 201, 181));
        video1->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 255);"));
        video2 = new QLabel(xvideoViewClass);
        video2->setObjectName(QString::fromUtf8("video2"));
        video2->setGeometry(QRect(280, 20, 181, 171));
        video2->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 255, 0);"));
        open1 = new QPushButton(xvideoViewClass);
        open1->setObjectName(QString::fromUtf8("open1"));
        open1->setGeometry(QRect(160, 240, 75, 21));
        pix1 = new QComboBox(xvideoViewClass);
        pix1->addItem(QString());
        pix1->addItem(QString());
        pix1->addItem(QString());
        pix1->addItem(QString());
        pix1->setObjectName(QString::fromUtf8("pix1"));
        pix1->setGeometry(QRect(30, 240, 91, 22));
        width1 = new QSpinBox(xvideoViewClass);
        width1->setObjectName(QString::fromUtf8("width1"));
        width1->setGeometry(QRect(30, 290, 91, 22));
        width1->setMinimum(1);
        width1->setMaximum(99999);
        width1->setValue(400);
        height1 = new QSpinBox(xvideoViewClass);
        height1->setObjectName(QString::fromUtf8("height1"));
        height1->setGeometry(QRect(30, 320, 91, 22));
        height1->setMinimum(1);
        height1->setMaximum(99997);
        height1->setValue(300);
        set_fps1 = new QSpinBox(xvideoViewClass);
        set_fps1->setObjectName(QString::fromUtf8("set_fps1"));
        set_fps1->setGeometry(QRect(30, 360, 91, 22));
        set_fps1->setMinimum(1);
        set_fps1->setMaximum(1000);
        set_fps1->setValue(25);
        set_fps2 = new QSpinBox(xvideoViewClass);
        set_fps2->setObjectName(QString::fromUtf8("set_fps2"));
        set_fps2->setGeometry(QRect(290, 360, 91, 22));
        set_fps2->setMinimum(1);
        set_fps2->setMaximum(1000);
        set_fps2->setValue(25);
        height2 = new QSpinBox(xvideoViewClass);
        height2->setObjectName(QString::fromUtf8("height2"));
        height2->setGeometry(QRect(290, 320, 91, 22));
        height2->setMinimum(1);
        height2->setMaximum(99997);
        height2->setValue(300);
        pix2 = new QComboBox(xvideoViewClass);
        pix2->addItem(QString());
        pix2->addItem(QString());
        pix2->addItem(QString());
        pix2->addItem(QString());
        pix2->setObjectName(QString::fromUtf8("pix2"));
        pix2->setGeometry(QRect(290, 240, 91, 22));
        width2 = new QSpinBox(xvideoViewClass);
        width2->setObjectName(QString::fromUtf8("width2"));
        width2->setGeometry(QRect(290, 290, 91, 22));
        width2->setMinimum(1);
        width2->setMaximum(99999);
        width2->setValue(400);
        open2 = new QPushButton(xvideoViewClass);
        open2->setObjectName(QString::fromUtf8("open2"));
        open2->setGeometry(QRect(410, 240, 75, 23));
        fps1 = new QLabel(xvideoViewClass);
        fps1->setObjectName(QString::fromUtf8("fps1"));
        fps1->setGeometry(QRect(40, 30, 54, 12));
        fps1->setStyleSheet(QString::fromUtf8("color: rgb(255, 0, 25);"));
        fps2 = new QLabel(xvideoViewClass);
        fps2->setObjectName(QString::fromUtf8("fps2"));
        fps2->setGeometry(QRect(290, 30, 54, 12));
        fps2->setStyleSheet(QString::fromUtf8("color: rgb(255, 8, 24);"));

        retranslateUi(xvideoViewClass);
        QObject::connect(open1, SIGNAL(clicked()), xvideoViewClass, SLOT(Open1()));
        QObject::connect(open2, SIGNAL(clicked()), xvideoViewClass, SLOT(Open2()));

        QMetaObject::connectSlotsByName(xvideoViewClass);
    } // setupUi

    void retranslateUi(QWidget *xvideoViewClass)
    {
        xvideoViewClass->setWindowTitle(QCoreApplication::translate("xvideoViewClass", "xvideoView", nullptr));
        video1->setText(QCoreApplication::translate("xvideoViewClass", "video", nullptr));
        video2->setText(QCoreApplication::translate("xvideoViewClass", "video2", nullptr));
        open1->setText(QCoreApplication::translate("xvideoViewClass", "Open", nullptr));
        pix1->setItemText(0, QCoreApplication::translate("xvideoViewClass", "YUV420P", nullptr));
        pix1->setItemText(1, QCoreApplication::translate("xvideoViewClass", "RGBA", nullptr));
        pix1->setItemText(2, QCoreApplication::translate("xvideoViewClass", "ARGB", nullptr));
        pix1->setItemText(3, QCoreApplication::translate("xvideoViewClass", "BGRA", nullptr));

        pix2->setItemText(0, QCoreApplication::translate("xvideoViewClass", "YUV420P", nullptr));
        pix2->setItemText(1, QCoreApplication::translate("xvideoViewClass", "RGBA", nullptr));
        pix2->setItemText(2, QCoreApplication::translate("xvideoViewClass", "ARGB", nullptr));
        pix2->setItemText(3, QCoreApplication::translate("xvideoViewClass", "BGRA", nullptr));

        open2->setText(QCoreApplication::translate("xvideoViewClass", "Open", nullptr));
        fps1->setText(QCoreApplication::translate("xvideoViewClass", "FPS:", nullptr));
        fps2->setText(QCoreApplication::translate("xvideoViewClass", "FPS:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class xvideoViewClass: public Ui_xvideoViewClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_YUVRGBPLAYER_H
