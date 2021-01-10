#include "yuvrgbplayer.h"
#include <fstream>
#include <iostream>
#include <QMessageBox>
#include <thread>
#include <sstream>
#include <QSpinBox>
#include <QFileDialog>
#include <vector>
#include "xvideo_view.h"
using namespace std;

static std::vector<XVideoView*> views;
yuvRgbPlayer::yuvRgbPlayer(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	// 绑定渲染信号槽
	connect(this, SIGNAL(ViewS()), this, SLOT(View()));
	views.push_back(XVideoView::Create());
	views.push_back(XVideoView::Create());
	views[0]->set_win_id((void *)ui.video1->winId());
	views[1]->set_win_id((void *)ui.video2->winId());
	th_ = std::thread(&yuvRgbPlayer::Main, this);
}

void yuvRgbPlayer::timerEvent(QTimerEvent * ev)
{
}

void yuvRgbPlayer::resizeEvent(QResizeEvent * ev)
{
}
void yuvRgbPlayer::View()
{

}
void yuvRgbPlayer::Main()
{
	while (!is_exit)
	{
		ViewS();
		MSleep(10);
	}
}

void yuvRgbPlayer::Open1()
{
	Open(0);
}

void yuvRgbPlayer::Open2()
{
	Open(1);
}

void yuvRgbPlayer::Open(int i)
{
	QFileDialog fd;
	auto filename = fd.getOpenFileName();
	if (filename.isEmpty()) return;
	cout << filename.toLocal8Bit().data() << endl;
	// 打开文件
	if (!views[i]->Open(filename.toStdString()))
	{
		return;
	}
	int w = 0;
	int h = 0;
	QString pix = 0; // YUV420P RGBA
	if (i == 0) {
		w = ui.width1->value();
		h = ui.height1->value();
		pix = ui.pix1->currentText(); //像素格式
	}
	else {
		w = ui.width2->value();
		h = ui.height2->value();
		pix = ui.pix2->currentText();
	}
	XVideoView::Format fmt;
	if (pix == "YUV420P") {
		fmt = XVideoView::YUV420P;
	}
	else if (pix == "RGBA") {
		fmt = XVideoView::RGBA;
	}
	else if (pix == "ARGB") {
		fmt = XVideoView::ARGB;
	}
	else if (pix == "BGRA") {
		fmt = XVideoView::BGRA;
	}
	// 初始化窗口和材质
	views[i]->Init(w, h, fmt);
}
