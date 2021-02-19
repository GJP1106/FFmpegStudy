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
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_XViewerClass
{
public:

    void setupUi(QWidget *XViewerClass)
    {
        if (XViewerClass->objectName().isEmpty())
            XViewerClass->setObjectName(QString::fromUtf8("XViewerClass"));
        XViewerClass->resize(600, 400);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/XViewer/img/xv.ico"), QSize(), QIcon::Normal, QIcon::Off);
        XViewerClass->setWindowIcon(icon);

        retranslateUi(XViewerClass);

        QMetaObject::connectSlotsByName(XViewerClass);
    } // setupUi

    void retranslateUi(QWidget *XViewerClass)
    {
        XViewerClass->setWindowTitle(QCoreApplication::translate("XViewerClass", "XViewer", nullptr));
    } // retranslateUi

};

namespace Ui {
    class XViewerClass: public Ui_XViewerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_XVIEWER_H
