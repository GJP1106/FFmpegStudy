#pragma once

#include <QtWidgets/QWidget>
#include "ui_sdl_qt_rgb.h"

class Sdl_Qt_Rgb : public QWidget
{
	Q_OBJECT

public:
	Sdl_Qt_Rgb(QWidget *parent = Q_NULLPTR);
	void timerEvent(QTimerEvent* ev) override;

private:
	Ui::Sdl_Qt_RgbClass ui;
};
