#pragma once

#include <QtWidgets/QWidget>
#include "ui_sdl_qt_play_yuv.h"

class sdl_qt_play_yuv : public QWidget
{
	Q_OBJECT

public:
	sdl_qt_play_yuv(QWidget *parent = Q_NULLPTR);
	void timerEvent(QTimerEvent* ev) override;

private:
	Ui::sdl_qt_play_yuvClass ui;
};
