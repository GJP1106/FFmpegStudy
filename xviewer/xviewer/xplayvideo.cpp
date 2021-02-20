#include "xplayvideo.h"

XPlayVideo::XPlayVideo(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	startTimer(10);
}

XPlayVideo::~XPlayVideo()
{
	Close();
}

bool XPlayVideo::Open(const char * url)
{
	// 关闭上次数据
	Close();
	if (!demux_.Open(url)) {	//解封装
		return false;
	}
	auto vp = demux_.CopyVideoPara();
	if (!vp) return false;
	if (!decode_.Open(vp->para)) {  //解码
		return false;
	}
	auto ap = demux_.CopyAudioPara();
	demux_.set_next(&decode_);
	if (!view_) {
		view_ = XVideoView::Create();
	}
	view_->set_win_id((void*)winId());
	if (!view_->Init(vp->para)) {		//SDL渲染
		return false;
	}
	demux_.Start();
	decode_.Start();

	return true;
}

void XPlayVideo::timerEvent(QTimerEvent * ev)
{
	if (!view_) return;
	auto f = decode_.GetFrame();
	if (!f) return;
	view_->DrawFrame(f);
	XFreeFrame(&f);
}

void XPlayVideo::Close()
{
	// 关闭上次数据
	demux_.Stop();
	decode_.Stop();
	if (view_) {
		view_->Close();
		delete view_;
		view_ = nullptr;
	}
}

void XPlayVideo::closeEvent(QCloseEvent * ev)
{
	Close();
}
