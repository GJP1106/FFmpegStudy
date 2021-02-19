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
//�����������
#define C(s) QString::fromLocal8Bit(s)

static QWidget *cam_wids[16] = { 0 };
XViewer::XViewer(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	// ȥ��ԭ���ڱ߿�
	setWindowFlags(Qt::FramelessWindowHint);
	//����head �� body ��ֱ������
	auto vlay = new QVBoxLayout();
	// �߿���
	vlay->setContentsMargins(0, 0, 0, 0);
	// Ԫ�ؼ��
	vlay->setSpacing(0);
	vlay->addWidget(ui.head);
	vlay->addWidget(ui.body);
	this->setLayout(vlay);

	//����б� �����Ԥ��
	//ˮƽ������
	auto hlay = new QHBoxLayout();
	ui.body->setLayout(hlay);
	// �߿���
	vlay->setContentsMargins(0, 0, 0, 0);
	hlay->addWidget(ui.left);
	hlay->addWidget(ui.cams);

	////////////////////////////////
	// ��ʼ���Ҽ��˵�
	// ��ͼ-->1����
	//        4����
	auto m = left_menu_.addMenu(C("��ͼ"));
	auto a = m->addAction(C("1����"));
	connect(a, SIGNAL(triggered()), this, SLOT(View1()));
	a = m->addAction(C("4����"));
	connect(a, SIGNAL(triggered()), this, SLOT(View4()));
	a = m->addAction(C("9����"));
	connect(a, SIGNAL(triggered()), this, SLOT(View9()));
	a = m->addAction(C("16����"));
	connect(a, SIGNAL(triggered()), this, SLOT(View16()));

	// Ĭ�ϴ���
	View(9);

	// ˢ���������б�
	XCameraConfig::Instance()->Load(CAM_CONF_PATH);
	RefreshCams();
}

// ����϶������¼�

static bool mouse_press = false;
//��걻���µ�λ��
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

// ���ڴ�С�����仯
void XViewer::resizeEvent(QResizeEvent * ev)
{
	int x = width() - ui.head_button->width();
	int y = ui.head_button->y();
	ui.head_button->move(x, y);
}

void XViewer::contextMenuEvent(QContextMenuEvent * event)
{
	// ���λ����ʾ�Ҽ��˵�
	left_menu_.exec(QCursor::pos());
	event->accept();
}

void XViewer::View(int count)
{
	qDebug() << "View" << count;
	// 2x2 3x3 4x4
	// ȷ������ ����
	int cols = sqrt(count);
	// �ܴ�������
	int wid_size = sizeof(cam_wids) / sizeof(QWidget*);
	//��ʼ��������
	auto lay = (QGridLayout*)ui.cams->layout();
	if (!lay) {
		lay = new QGridLayout();
		lay->setContentsMargins(0, 0, 0, 0);
		lay->setSpacing(2);		//Ԫ�ؼ��
		ui.cams->setLayout(lay);
	}
	//��ʼ������
	for (int i = 0; i < count; i++) {
		if (!cam_wids[i]) {
			cam_wids[i] = new QWidget();
			cam_wids[i]->setStyleSheet("background-color:rgb(51, 51, 51);");
		}
		lay->addWidget(cam_wids[i], i / cols, i % cols);
	}
	// �������Ĵ���
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
		QMessageBox::information(this, "error", C("��ѡ�������"));
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
	// ����1 �����1
	QLineEdit name_edit;
	lay.addRow(C("����"), &name_edit);
	QLineEdit url_edit;
	lay.addRow(C("������"), &url_edit);
	QLineEdit sub_url_edit;
	lay.addRow(C("������"), &sub_url_edit);
	QLineEdit save_path_edit;
	lay.addRow(C("����Ŀ¼"), &save_path_edit);

	QPushButton save;
	save.setText(C("����"));
	connect(&save, SIGNAL(clicked()), &dlg, SLOT(accept()));
	lay.addRow("", &save);

	// �༭ ����ԭ������ʾ
	if (index > 0) {
		auto cam = c->GetCam(index);
		name_edit.setText(C(cam.name));
		url_edit.setText(C(cam.url));
		sub_url_edit.setText(C(cam.sub_url));
		save_path_edit.setText(C(cam.save_path));
	}

	for (;;) {
		if (dlg.exec() == QDialog::Accepted) {    // ����˱���
			if (name_edit.text().isEmpty()) {
				QMessageBox::information(0, "error", C("����������"));
				continue;
			}
			if (url_edit.text().isEmpty()) {
				QMessageBox::information(0, "error", C("������������"));
				continue;
			}
			if (sub_url_edit.text().isEmpty()) {
				QMessageBox::information(0, "error", C("�����븨����"));
				continue;
			}
			if (save_path_edit.text().isEmpty()) {
				QMessageBox::information(0, "error", C("�����뱣��Ŀ¼"));
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
	if (index > 0) {		//�޸�
		c->SetCam(index, data);
	}
	else {		// ����
		c->Push(data);			//��������
	}
	c->Save(CAM_CONF_PATH);	//���浽�ļ�
	RefreshCams();	//ˢ����ʾ
}

void XViewer::DelCam()
{
	int row = ui.cam_list->currentIndex().row();
	if (row < 0) {
		QMessageBox::information(this, "error", C("��ѡ�������"));
		return;
	}
	stringstream ss;
	ss << "��ȷ��ɾ�������" << ui.cam_list->currentItem()->text().toLocal8Bit().constData();
	ss << "��?";
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
