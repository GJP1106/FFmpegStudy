#include "xcamera_widget.h"
#include <QStyleOption>
#include <QPainter>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QDebug>
#include <QListWidget>

XCameraWidget::XCameraWidget(QWidget * p) : QWidget(p)
{
	// 接收拖拽
	this->setAcceptDrops(true);
}
//拖拽进入
void XCameraWidget::dragEnterEvent(QDragEnterEvent * e)
{
	e->acceptProposedAction();
}

//拖拽松开
void XCameraWidget::dropEvent(QDropEvent * e)
{
	// 拿到url
	qDebug() << e->source()->objectName();
	auto wid = (QListWidget*)e->source();
	qDebug() << wid->currentRow();
}

//渲染
void XCameraWidget::paintEvent(QPaintEvent * p)
{
	// 渲染样式表
	QStyleOption opt;
	opt.init(this);
	QPainter painter(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}
