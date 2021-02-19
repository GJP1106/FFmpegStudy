#pragma once

#include <QtWidgets/QWidget>
#include "ui_xviewer.h"
#include <QMenu>

class XViewer : public QWidget
{
	Q_OBJECT

public:
	XViewer(QWidget *parent = Q_NULLPTR);

	//����¼��������϶�����
	void mouseMoveEvent(QMouseEvent* ev) override;
	void mousePressEvent(QMouseEvent* ev) override;
	void mouseReleaseEvent(QMouseEvent* ev) override;

	//���ڴ�С�����仯
	void resizeEvent(QResizeEvent* ev) override;
	//�Ҽ��˵�
	void contextMenuEvent(QContextMenuEvent* event) override;

	void View(int count);
public slots:
	void MaxWindow();
	void NormalWindow();
	void View1();
	void View4();
	void View9();
	void View16();

private:
	Ui::XViewerClass ui;
	QMenu left_menu_;
};
