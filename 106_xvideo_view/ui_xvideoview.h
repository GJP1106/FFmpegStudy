/********************************************************************************
** Form generated from reading UI file 'xvideoview.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_XVIDEOVIEW_H
#define UI_XVIDEOVIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_xvideoViewClass
{
public:
    QLabel *label;

    void setupUi(QWidget *xvideoViewClass)
    {
        if (xvideoViewClass->objectName().isEmpty())
            xvideoViewClass->setObjectName(QString::fromUtf8("xvideoViewClass"));
        xvideoViewClass->resize(600, 400);
        label = new QLabel(xvideoViewClass);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(70, 40, 241, 161));

        retranslateUi(xvideoViewClass);

        QMetaObject::connectSlotsByName(xvideoViewClass);
    } // setupUi

    void retranslateUi(QWidget *xvideoViewClass)
    {
        xvideoViewClass->setWindowTitle(QCoreApplication::translate("xvideoViewClass", "xvideoView", nullptr));
        label->setText(QCoreApplication::translate("xvideoViewClass", "video", nullptr));
    } // retranslateUi

};

namespace Ui {
    class xvideoViewClass: public Ui_xvideoViewClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_XVIDEOVIEW_H
