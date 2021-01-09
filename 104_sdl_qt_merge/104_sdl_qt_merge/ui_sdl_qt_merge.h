/********************************************************************************
** Form generated from reading UI file 'sdl_qt_merge.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SDL_QT_MERGE_H
#define UI_SDL_QT_MERGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_sdl_qt_mergeClass
{
public:
    QLabel *label;

    void setupUi(QWidget *sdl_qt_mergeClass)
    {
        if (sdl_qt_mergeClass->objectName().isEmpty())
            sdl_qt_mergeClass->setObjectName(QString::fromUtf8("sdl_qt_mergeClass"));
        sdl_qt_mergeClass->resize(600, 400);
        label = new QLabel(sdl_qt_mergeClass);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(63, 71, 181, 211));

        retranslateUi(sdl_qt_mergeClass);

        QMetaObject::connectSlotsByName(sdl_qt_mergeClass);
    } // setupUi

    void retranslateUi(QWidget *sdl_qt_mergeClass)
    {
        sdl_qt_mergeClass->setWindowTitle(QCoreApplication::translate("sdl_qt_mergeClass", "sdl_qt_merge", nullptr));
        label->setText(QCoreApplication::translate("sdl_qt_mergeClass", "video", nullptr));
    } // retranslateUi

};

namespace Ui {
    class sdl_qt_mergeClass: public Ui_sdl_qt_mergeClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SDL_QT_MERGE_H
