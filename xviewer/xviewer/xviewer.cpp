#include "xviewer.h"
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QResizeEvent>
#include <QDebug>
#include <QContextMenuEvent>
#include <QGridLayout>
#include "xcamera_config.h"
#include <QListWidgetItem>
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <sstream>
using namespace std;
#define CAM_CONF_PATH "test.db"
//解决中文乱码
#define C(s) QString::fromLocal8Bit(s)

static QWidget *cam_wids[16] = { 0 };
XViewer::XViewer(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	// 去除原窗口边框
	setWindowFlags(Qt::FramelessWindowHint);
	//布局head 和 body 垂直布局器
	auto vlay = new QVBoxLayout();
	// 边框间距
	vlay->setContentsMargins(0, 0, 0, 0);
	// 元素间距
	vlay->setSpacing(0);
	vlay->addWidget(ui.head);
	vlay->addWidget(ui.body);
	this->setLayout(vlay);

	//相机列表 和相机预览
	//水平布局器
	auto hlay = new QHBoxLayout();
	ui.body->setLayout(hlay);
	// 边框间距
	vlay->setContentsMargins(0, 0, 0, 0);
	hlay->addWidget(ui.left);
	hlay->addWidget(ui.cams);

	////////////////////////////////
	// 初始化右键菜单
	// 视图-->1窗口
	//        4窗口
	auto m = left_menu_.addMenu(C("视图"));
	auto a = m->addAction(C("1窗口"));
	connect(a, SIGNAL(triggered()), this, SLOT(View1()));
	a = m->addAction(C("4窗口"));
	connect(a, SIGNAL(triggered()), this, SLOT(View4()));
	a = m->addAction(C("9窗口"));
	connect(a, SIGNAL(triggered()), this, SLOT(View9()));
	a = m->addAction(C("16窗口"));
	connect(a, SIGNAL(triggered()), this, SLOT(View16()));

	// 默认窗口
	View(9);

	// 刷新左侧相机列表
	XCameraConfig::Instance()->Load(CAM_CONF_PATH);
	RefreshCams();
}

// 鼠标拖动窗口事件

static bool mouse_press = false;
//鼠标被按下的位置
static QPoint mouse_point;

void XViewer::mouseMoveEvent(QMouseEvent * ev)
{
	if (!mouse_press) {
		QWidget::mouseMoveEvent(ev);
		return;
	}
	this->move(ev->globalPos() - mouse_point);
}

void XViewer::mousePressEvent(QMouseEvent * ev)
{
	if (ev->button() == Qt::LeftButton) {
		mouse_press = true;
		mouse_point = ev->pos();
	}
}

void XViewer::mouseReleaseEvent(QMouseEvent * ev)
{
	mouse_press = false;
}

// 窗口大小发生变化
void XViewer::resizeEvent(QResizeEvent * ev)
{
	int x = width() - ui.head_button->width();
	int y = ui.head_button->y();
	ui.head_button->move(x, y);
}

void XViewer::contextMenuEvent(QContextMenuEvent * event)
{
	// 鼠标位置显示右键菜单
	left_menu_.exec(QCursor::pos());
	event->accept();
}

void XViewer::View(int count)
{
	qDebug() << "View" << count;
	// 2x2 3x3 4x4
	// 确定列数 根号
	int cols = sqrt(count);
	// 总窗口数量
	int wid_size = sizeof(cam_wids) / sizeof(QWidget*);
	//初始化布局器
	auto lay = (QGridLayout*)ui.cams->layout();
	if (!lay) {
		lay = new QGridLayout();
		lay->setContentsMargins(0, 0, 0, 0);
		lay->setSpacing(2);		//元素间距
		ui.cams->setLayout(lay);
	}
	//初始化窗口
	for (int i = 0; i < count; i++) {
		if (!cam_wids[i]) {
			cam_wids[i] = new QWidget();
			cam_wids[i]->setStyleSheet("background-color:rgb(51, 51, 51);");
		}
		lay->addWidget(cam_wids[i], i / cols, i % cols);
	}
	// 清理多余的窗体
	for (int i = count; i < wid_size; i++) {
		if (cam_wids[i]) {
			delete cam_wids[i];
			cam_wids[i] = nullptr;
		}
	}
}

void XViewer::RefreshCams()
{
	auto c = XCameraConfig::Instance();
	ui.cam_list->clear();
	int count = c->GetCamCount();
	for (int i = 0; i < count; i++) {
		auto cam = c->GetCam(i);
		auto item = new QListWidgetItem(QIcon(":/XViewer/img/cam.png"), C(cam.name));
		ui.cam_list->addItem(item);
	}
}


void XViewer::MaxWindow()
{
	ui.max->setVisible(false);
	ui.normal->setVisible(true);
	showMaximized();
}
void XViewer::NormalWindow()
{
	ui.max->setVisible(true);
	ui.normal->setVisible(false);
	showNormal();
}

void XViewer::View1()
{
	View(1);
}
void XViewer::View4()
{
	View(4);
}
void XViewer::View9()
{
	View(9);
}
void XViewer::View16()
{
	View(16);
}

void XViewer::AddCam()
{
	SetCam(-1);
}

void XViewer::SetCam()
{
	int row = ui.cam_list->currentIndex().row();
	if (row < 0) {
		QMessageBox::information(this, "error", C("请选择摄像机"));
		return;
	}
	SetCam(row);
}
void XViewer::SetCam(int index)
{
	auto c = XCameraConfig::Instance();
	QDialog dlg(this);
	dlg.resize(800, 200);
	QFormLayout lay;
	dlg.setLayout(&lay);
	// 标题1 输入框1
	QLineEdit name_edit;
	lay.addRow(C("名称"), &name_edit);
	QLineEdit url_edit;
	lay.addRow(C("主码流"), &url_edit);
	QLineEdit sub_url_edit;
	lay.addRow(C("辅码流"), &sub_url_edit);
	QLineEdit save_path_edit;
	lay.addRow(C("保存目录"), &save_path_edit);

	QPushButton save;
	save.setText(C("保存"));
	connect(&save, SIGNAL(clicked()), &dlg, SLOT(accept()));
	lay.addRow("", &save);

	// 编辑 读入原数据显示
	if (index > 0) {
		auto cam = c->GetCam(index);
		name_edit.setText(C(cam.name));
		url_edit.setText(C(cam.url));
		sub_url_edit.setText(C(cam.sub_url));
		save_path_edit.setText(C(cam.save_path));
	}

	for (;;) {
		if (dlg.exec() == QDialog::Accepted) {    // 点击了保存
			if (name_edit.text().isEmpty()) {
				QMessageBox::information(0, "error", C("请输入名称"));
				continue;
			}
			if (url_edit.text().isEmpty()) {
				QMessageBox::information(0, "error", C("请输入主码流"));
				continue;
			}
			if (sub_url_edit.text().isEmpty()) {
				QMessageBox::information(0, "error", C("请输入辅码流"));
				continue;
			}
			if (save_path_edit.text().isEmpty()) {
				QMessageBox::information(0, "error", C("请输入保存目录"));
				continue;
			}
			break;
		}
		return;
	}
	XCameraData data;
	strcpy(data.name, name_edit.text().toLocal8Bit());
	strcpy(data.url, url_edit.text().toLocal8Bit());
	strcpy(data.sub_url, sub_url_edit.text().toLocal8Bit());
	strcpy(data.save_path, save_path_edit.text().toLocal8Bit());
	if (index > 0) {		//修改
		c->SetCam(index, data);
	}
	else {		// 新增
		c->Push(data);			//插入数据
	}
	c->Save(CAM_CONF_PATH);	//保存到文件
	RefreshCams();	//刷新显示
}

void XViewer::DelCam()
{
	int row = ui.cam_list->currentIndex().row();
	if (row < 0) {
		QMessageBox::information(this, "error", C("请选择摄像机"));
		return;
	}
	stringstream ss;
	ss << "您确认删除摄像机" << ui.cam_list->currentItem()->text().toLocal8Bit().constData();
	ss << "吗?";
	if (QMessageBox::information(this,
		"confirm",
		C(ss.str().c_str()),
		QMessageBox::Yes,
		QMessageBox::No) != QMessageBox::Yes) {
		return;
	}
	XCameraConfig::Instance()->DelCam(row);
	RefreshCams();
}
