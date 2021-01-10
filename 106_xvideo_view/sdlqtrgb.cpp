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
static QLabel * view_fps = nullptr;  //显示fps控件
static QSpinBox* set_fps = nullptr;  //设置fps控件
int fps = 25;   // 播放帧率
sdlqtrgb::sdlqtrgb(QWidget *parent)
	: QWidget(parent)
{
	// 打开yuv文件
	yuv_file.open("400_300_25.yuv", ios::binary);
	if (!yuv_file) {
		QMessageBox::information(this, "", "open yuv failed!");
		return;
	}
	yuv_file.seekg(0, ios::end); //移到文件结尾
	file_size = yuv_file.tellg(); //文件指针位置
	//yuv_file.seekg(0, ios::beg);
	ui.setupUi(this);

	// 绑定渲染信号槽
	connect(this, SIGNAL(ViewS()), this, SLOT(View()));

	// 显示fps的控件
	view_fps = new QLabel(this);
	view_fps->setText("100fps");

	//设置fps
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

	//生成frame对象空间
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

	// 生成图像空间， 默认32字节对齐
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

	if (yuv_file.tellg() == file_size) { //读取到文件结尾
		yuv_file.seekg(0, ios::beg);
	}
	//yuv_file.gcount();
	//yuv_file.seekg(); //结尾处seek无效
	if (view->IsExit()) {
		view->Close();
		exit(0);
	}
	// yuv平面存储
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
	if (yuv_file.tellg() == file_size) { //读取到文件结尾
		yuv_file.seekg(0, ios::beg);
	}
	yuv_file.read((char *)frame->data[0], sdl_width * sdl_height); // Y
	yuv_file.read((char *)frame->data[1], sdl_width * sdl_height / 4); // U
	yuv_file.read((char *)frame->data[2], sdl_width * sdl_height / 4); // V

	if (view->IsExit()) {
		view->Close();
		exit(0);
	}
	// yuv平面存储
	// yyyy yyyy uu vv
	//yuv_file.read((char *)yuv, sdl_width * sdl_height * 1.5);
	view->DrawFrame(frame);
	stringstream ss;
	ss << "fps:" << view->render_fps();
	// 只能在槽函数中调用
	view_fps->setText(ss.str().c_str());
	fps = set_fps->value(); // 拿到播放帧率
}
