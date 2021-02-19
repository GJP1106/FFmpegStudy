#pragma once
//#include "E:\Qt5.14.2\5.14.2\msvc2017\include\QtWidgets\qwidget.h"
#include <QWidget>
class XCameraWidget : public QWidget
{
	Q_OBJECT
public:
	XCameraWidget(QWidget* p = nullptr);
	//мов╖╫ЬхК
	void dragEnterEvent(QDragEnterEvent* e) override;

	//мов╖ки©╙
	void dropEvent(QDropEvent* e) override;

	//Джх╬
	void paintEvent(QPaintEvent* p);
};

