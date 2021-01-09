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
		// �ȴ���Ⱦ�߳��˳�
		th_.join();
	}
	void timerEvent(QTimerEvent *ev) override;
	// QT�����غ���
	void resizeEvent(QResizeEvent* ev) override;
	// �̺߳���������ˢ����Ƶ
	void Main();
	// �źź���
signals:
	void ViewS();  //�źź���������������б�
public slots:
	void View();  //��ʾ�Ĳۺ���

private:
	std::thread th_;
	bool is_exit = false; // �����߳��˳�
	Ui::xvideoViewClass ui;
};
