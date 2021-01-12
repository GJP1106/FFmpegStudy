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
	//存放上次渲染的时间戳
	static int last_pts[32] = { 0 };
	static int fps_arr[32] = { 0 };
	fps_arr[0] = ui.set_fps1->value();
	fps_arr[1] = ui.set_fps2->value();
	for (int i = 0; i < views.size(); i++) {
		if (fps_arr[i] <= 0) continue;
		// 需要间隔时间
		int ms = 1000 / fps_arr[i];
		// 判断是否到了可渲染时间
		if (NowMs() - last_pts[i] < ms) {
			continue;
		}
		last_pts[i] = NowMs();
		auto frame = views[i]->Read();
		if (!frame) {
			continue;
		}
		views[i]->DrawFrame(frame);
		// 显示fps
		stringstream ss;
		ss << "fps:" << views[i]->render_fps();
		if (i == 0) {
			ui.fps1->setText(ss.str().c_str());
		}
		else {
			ui.fps2->setText(ss.str().c_str());
		}
	}
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
		cout << "Open file failed!" << endl;
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
	else if (pix == "RGB24") {
		fmt = XVideoView::RGB24;
	}
	// 初始化窗口和材质
	views[i]->Init(w, h, fmt);
}
