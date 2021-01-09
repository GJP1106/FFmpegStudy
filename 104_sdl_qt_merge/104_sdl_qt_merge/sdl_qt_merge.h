#pragma once

#include <QtWidgets/QWidget>
#include "ui_sdl_qt_merge.h"

class sdl_qt_merge : public QWidget
{
	Q_OBJECT

public:
	sdl_qt_merge(QWidget *parent = Q_NULLPTR);
	void timerEvent(QTimerEvent* ev) override;

private:
	Ui::sdl_qt_mergeClass ui;
};
