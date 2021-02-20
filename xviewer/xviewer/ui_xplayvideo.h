/********************************************************************************
** Form generated from reading UI file 'xplayvideo.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_XPLAYVIDEO_H
#define UI_XPLAYVIDEO_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_XPlayVideo
{
public:

    void setupUi(QWidget *XPlayVideo)
    {
        if (XPlayVideo->objectName().isEmpty())
            XPlayVideo->setObjectName(QString::fromUtf8("XPlayVideo"));
        XPlayVideo->resize(733, 525);

        retranslateUi(XPlayVideo);

        QMetaObject::connectSlotsByName(XPlayVideo);
    } // setupUi

    void retranslateUi(QWidget *XPlayVideo)
    {
        XPlayVideo->setWindowTitle(QCoreApplication::translate("XPlayVideo", "XPlayVideo", nullptr));
    } // retranslateUi

};

namespace Ui {
    class XPlayVideo: public Ui_XPlayVideo {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_XPLAYVIDEO_H
