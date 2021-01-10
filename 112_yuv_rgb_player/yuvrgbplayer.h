#pragma once

#include <QtWidgets/QWidget>
#include "ui_yuvrgbplayer.h"
#include <thread>

class yuvRgbPlayer : public QWidget
{
	Q_OBJECT

public:
	yuvRgbPlayer(QWidget *parent = Q_NULLPTR);
	~yuvRgbPlayer() {
		is_exit = true;
		// 等待渲染线程退出
		th_.join();
	}
	void timerEvent(QTimerEvent* ev) override;
	void resizeEvent(QResizeEvent* ev) override;
	//线程函数，用于刷新视频
	void Main();

signals:
	void ViewS();		//信号函数，将任务放入列表
public slots:
	void View();		// 显示的槽函数
	void Open1();
	void Open2();
	void Open(int i);
private:
	std::thread th_;
	bool is_exit = false;  //处理线程退出

	Ui::xvideoViewClass ui;
};
