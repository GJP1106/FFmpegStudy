#pragma once

#include <QtWidgets/QWidget>
#include "ui_xvideoview.h"

class sdlqtrgb : public QWidget
{
	Q_OBJECT

public:
	sdlqtrgb(QWidget *parent = Q_NULLPTR);
	void timerEvent(QTimerEvent *ev) override;
	// QTµÄÖØÔØº¯Êı
	void resizeEvent(QResizeEvent* ev) override;

private:
	Ui::xvideoViewClass ui;
};
