#include "sdlqtrgb.h"
//#include <sdl/SDL.h>
#include <QtWidgets/QMessageBox>
#include <iostream>
#include <fstream>
#include <thread>
#include <sstream>
#include <QSpinBOX>
#include "xvideo_view.h"
extern "C"
{
#include <libavcodec/avcodec.h>
}
using namespace std;

// #pragma comment(lib, "SDL2.lib")
// #pragma comment(lib, "avutil.lib")

static int sdl_width = 0;
static int sdl_height = 0;
static int pix_size = 2;
static ifstream yuv_file;
static XVideoView* view = nullptr;
static AVFrame *frame = nullptr;
static long long file_size = 0;
static QLabel * view_fps = nullptr;  //��ʾfps�ؼ�
static QSpinBox* set_fps = nullptr;  //����fps�ؼ�
int fps = 25;   // ����֡��
sdlqtrgb::sdlqtrgb(QWidget *parent)
	: QWidget(parent)
{
	// ��yuv�ļ�
	yuv_file.open("400_300_25.yuv", ios::binary);
	if (!yuv_file) {
		QMessageBox::information(this, "", "open yuv failed!");
		return;
	}
	yuv_file.seekg(0, ios::end); //�Ƶ��ļ���β
	file_size = yuv_file.tellg(); //�ļ�ָ��λ��
	//yuv_file.seekg(0, ios::beg);
	ui.setupUi(this);

	// ����Ⱦ�źŲ�
	connect(this, SIGNAL(ViewS()), this, SLOT(View()));

	// ��ʾfps�Ŀؼ�
	view_fps = new QLabel(this);
	view_fps->setText("100fps");

	//����fps
	set_fps = new QSpinBox(this);
	set_fps->move(200, 0);
	set_fps->setValue(25);
	set_fps->setRange(1, 300);

	sdl_width = 400; //ui.label->width();
	sdl_height = 300; // ui.label->height();
	ui.label->resize(sdl_width, sdl_height);
	view = XVideoView::Create();
	view->Init(sdl_width, sdl_height, XVideoView::YUV420P/*, (void*)ui.label->winId()*/);
	view->Close();
	view->Init(sdl_width, sdl_height, XVideoView::YUV420P, (void*)ui.label->winId());

	//����frame����ռ�
	frame = av_frame_alloc();
	frame->width = sdl_width;
	frame->height = sdl_height;
	frame->format = AV_PIX_FMT_YUV420P;
	///////////////////////
	//  Y  Y
	//   UV
	//  Y  Y
	frame->linesize[0] = sdl_width;  // Y
	frame->linesize[1] = sdl_width / 2;  // U
	frame->linesize[2] = sdl_width / 2;  // V

	// ����ͼ��ռ䣬 Ĭ��32�ֽڶ���
	auto re = av_frame_get_buffer(frame, 0);
	if (re != 0) {
		char buf[1024] = { 0 };
		av_strerror(re, buf, sizeof(buf));
		cout << buf << endl;
	}


	//startTimer(10);
	th_ = std::thread(&sdlqtrgb::Main, this);
;}

void sdlqtrgb::timerEvent(QTimerEvent * ev)
{
	// yuv420p
	// 4 * 2
	// yyyy yyyy
	// u    u
	// v    v

	yuv_file.read((char *)frame->data[0], sdl_width * sdl_height); // Y
	yuv_file.read((char *)frame->data[1], sdl_width * sdl_height / 4); // U
	yuv_file.read((char *)frame->data[2], sdl_width * sdl_height / 4); // V

	if (yuv_file.tellg() == file_size) { //��ȡ���ļ���β
		yuv_file.seekg(0, ios::beg);
	}
	//yuv_file.gcount();
	//yuv_file.seekg(); //��β��seek��Ч
	if (view->IsExit()) {
		view->Close();
		exit(0);
	}
	// yuvƽ��洢
	// yyyy yyyy uu vv
	//yuv_file.read((char *)yuv, sdl_width * sdl_height * 1.5);
	view->DrawFrame(frame);
	//view->Draw(yuv_1);
	//view_fps->setText("100fps");

}

void sdlqtrgb::resizeEvent(QResizeEvent * ev)
{
	ui.label->resize(size());
	ui.label->move(0, 0);
	//view->Scale(width(), height());
}

void sdlqtrgb::Main()
{
	while (!is_exit) {
		ViewS();
		//this_thread::sleep_for(40ms);
		if (fps > 0) {
			MSleep(1000 / fps);
		}
		else {
			MSleep(10);
		}
	}
}
void sdlqtrgb::View()
{
	if (yuv_file.tellg() == file_size) { //��ȡ���ļ���β
		yuv_file.seekg(0, ios::beg);
	}
	yuv_file.read((char *)frame->data[0], sdl_width * sdl_height); // Y
	yuv_file.read((char *)frame->data[1], sdl_width * sdl_height / 4); // U
	yuv_file.read((char *)frame->data[2], sdl_width * sdl_height / 4); // V

	if (view->IsExit()) {
		view->Close();
		exit(0);
	}
	// yuvƽ��洢
	// yyyy yyyy uu vv
	//yuv_file.read((char *)yuv, sdl_width * sdl_height * 1.5);
	view->DrawFrame(frame);
	stringstream ss;
	ss << "fps:" << view->render_fps();
	// ֻ���ڲۺ����е���
	view_fps->setText(ss.str().c_str());
	fps = set_fps->value(); // �õ�����֡��
}
