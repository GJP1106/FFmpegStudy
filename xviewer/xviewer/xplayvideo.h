#pragma once

#include <QDialog>
#include "ui_xplayvideo.h"
#include "xplayer.h"

class XPlayVideo : public QDialog
{
	Q_OBJECT

public:
	XPlayVideo(QWidget *parent = Q_NULLPTR);
	~XPlayVideo();
	bool Open(const char* url);
	void timerEvent(QTimerEvent* ev) override;
	void Close();
	void closeEvent(QCloseEvent* ev) override;
private:
	Ui::XPlayVideo ui;
	XPlayer player;
};
