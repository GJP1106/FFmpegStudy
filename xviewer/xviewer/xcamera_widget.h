#pragma once
//#include "E:\Qt5.14.2\5.14.2\msvc2017\include\QtWidgets\qwidget.h"
#include <QWidget>
class XCameraWidget : public QWidget
{
	Q_OBJECT
public:
	XCameraWidget(QWidget* p = nullptr);
	//��ק����
	void dragEnterEvent(QDragEnterEvent* e) override;

	//��ק�ɿ�
	void dropEvent(QDropEvent* e) override;

	//��Ⱦ
	void paintEvent(QPaintEvent* p);
};

