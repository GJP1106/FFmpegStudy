#pragma once

#include <QWidget>
#include "ui_xplayvideo.h"
#include "xdemuxtask.h"
#include "xdecodetask.h"
#include "xvideo_view.h"

class XPlayVideo : public QWidget
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
	XDemuxTask demux_;
	XDecodeTask decode_;
	XVideoView* view_ = nullptr;
};
