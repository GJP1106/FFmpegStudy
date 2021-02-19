#include "xcamera_widget.h"
#include <QStyleOption>
#include <QPainter>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QDebug>
#include <QListWidget>

XCameraWidget::XCameraWidget(QWidget * p) : QWidget(p)
{
	// ������ק
	this->setAcceptDrops(true);
}
//��ק����
void XCameraWidget::dragEnterEvent(QDragEnterEvent * e)
{
	e->acceptProposedAction();
}

//��ק�ɿ�
void XCameraWidget::dropEvent(QDropEvent * e)
{
	// �õ�url
	qDebug() << e->source()->objectName();
	auto wid = (QListWidget*)e->source();
	qDebug() << wid->currentRow();
}

//��Ⱦ
void XCameraWidget::paintEvent(QPaintEvent * p)
{
	// ��Ⱦ��ʽ��
	QStyleOption opt;
	opt.init(this);
	QPainter painter(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}
