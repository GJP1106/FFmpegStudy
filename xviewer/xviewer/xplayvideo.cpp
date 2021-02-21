#include "xplayvideo.h"
#include <QDebug>

XPlayVideo::XPlayVideo(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	startTimer(10);
}

XPlayVideo::~XPlayVideo()
{
	Close();
}
void XPlayVideo::SetSpeed()
{
	float speed = 1;
	int s = ui.speed->value();
	if (s <= 10) {
		speed = (float)s / (float)10;
	}
	else {
		speed = s - 9;
	}
	ui.speedtxt->setText(QString::number(speed));
	player.SetSpeed(speed);
}
bool XPlayVideo::Open(const char * url)
{
	if (!player.Open(url, (void*)ui.video->winId())) {
		return false;
	}
	player.Start();
	startTimer(10);
	//// �ر��ϴ�����
	//Close();
	//if (!demux_.Open(url)) {	//���װ
	//	return false;
	//}
	//auto vp = demux_.CopyVideoPara();
	//if (!vp) return false;
	//if (!decode_.Open(vp->para)) {  //����
	//	return false;
	//}
	//auto ap = demux_.CopyAudioPara();
	//demux_.set_next(&decode_);
	//if (!view_) {
	//	view_ = XVideoView::Create();
	//}
	//view_->set_win_id((void*)winId());
	//if (!view_->Init(vp->para)) {		//SDL��Ⱦ
	//	return false;
	//}
	//demux_.set_syn_type(XSYN_VIDEO);
	//demux_.Start();
	//decode_.Start();

	return true;
}

void XPlayVideo::timerEvent(QTimerEvent * ev)
{
	player.Update();
	auto pos = player.pos_ms();
	auto total = player.total_ms();
	ui.pos->setMaximum(total);
	ui.pos->setValue(pos);
	//if (!view_) return;
	//auto f = decode_.GetFrame();
	//if (!f) return;
	//view_->DrawFrame(f);
	//XFreeFrame(&f);
}

void XPlayVideo::Close()
{
	player.Stop();
	//// �ر��ϴ�����
	//demux_.Stop();
	//decode_.Stop();
	//if (view_) {
	//	view_->Close();
	//	delete view_;
	//	view_ = nullptr;
	//}
}

void XPlayVideo::closeEvent(QCloseEvent * ev)
{
	Close();
}
