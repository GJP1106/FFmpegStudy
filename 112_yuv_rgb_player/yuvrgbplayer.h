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
		// �ȴ���Ⱦ�߳��˳�
		th_.join();
	}
	void timerEvent(QTimerEvent* ev) override;
	void resizeEvent(QResizeEvent* ev) override;
	//�̺߳���������ˢ����Ƶ
	void Main();

signals:
	void ViewS();		//�źź���������������б�
public slots:
	void View();		// ��ʾ�Ĳۺ���
	void Open1();
	void Open2();
	void Open(int i);
private:
	std::thread th_;
	bool is_exit = false;  //�����߳��˳�

	Ui::xvideoViewClass ui;
};
