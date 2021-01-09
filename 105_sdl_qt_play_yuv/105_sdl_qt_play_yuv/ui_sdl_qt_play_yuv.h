/********************************************************************************
** Form generated from reading UI file 'sdl_qt_play_yuv.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SDL_QT_PLAY_YUV_H
#define UI_SDL_QT_PLAY_YUV_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_sdl_qt_play_yuvClass
{
public:
    QLabel *label;

    void setupUi(QWidget *sdl_qt_play_yuvClass)
    {
        if (sdl_qt_play_yuvClass->objectName().isEmpty())
            sdl_qt_play_yuvClass->setObjectName(QString::fromUtf8("sdl_qt_play_yuvClass"));
        sdl_qt_play_yuvClass->resize(600, 400);
        label = new QLabel(sdl_qt_play_yuvClass);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(53, -1, 211, 331));

        retranslateUi(sdl_qt_play_yuvClass);

        QMetaObject::connectSlotsByName(sdl_qt_play_yuvClass);
    } // setupUi

    void retranslateUi(QWidget *sdl_qt_play_yuvClass)
    {
        sdl_qt_play_yuvClass->setWindowTitle(QCoreApplication::translate("sdl_qt_play_yuvClass", "sdl_qt_play_yuv", nullptr));
        label->setText(QCoreApplication::translate("sdl_qt_play_yuvClass", "video", nullptr));
    } // retranslateUi

};

namespace Ui {
    class sdl_qt_play_yuvClass: public Ui_sdl_qt_play_yuvClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SDL_QT_PLAY_YUV_H
