#pragma once

#include <QtWidgets/QWidget>
#include "ui_yuvrgbplayer.h"

class yuvRgbPlayer : public QWidget
{
	Q_OBJECT

public:
	yuvRgbPlayer(QWidget *parent = Q_NULLPTR);

private:
	Ui::xvideoViewClass ui;
};
