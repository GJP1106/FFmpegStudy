#pragma once
#include <thread>
#include <QtWidgets/QWidget>
#include "ui_xvideoview.h"

class sdlqtrgb : public QWidget
{
	Q_OBJECT

public:
	sdlqtrgb(QWidget *parent = Q_NULLPTR);
	~sdlqtrgb() {
		is_exit = true;
		// 等待渲染线程退出
		th_.join();
	}
	void timerEvent(QTimerEvent *ev) override;
	// QT的重载函数
	void resizeEvent(QResizeEvent* ev) override;
	// 线程函数，用于刷新视频
	void Main();
	// 信号函数
signals:
	void ViewS();  //信号函数，将任务放入列表
public slots:
	void View();  //显示的槽函数

private:
	std::thread th_;
	bool is_exit = false; // 处理线程退出
	Ui::xvideoViewClass ui;
};
