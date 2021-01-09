#include "sdlqtrgb.h"
//#include <sdl/SDL.h>
#include <QtWidgets/QMessageBox>
#include <iostream>
#include <fstream>
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
sdlqtrgb::sdlqtrgb(QWidget *parent)
	: QWidget(parent)
{
	// 打开yuv文件
	yuv_file.open("400_300_25.yuv", ios::binary);
	if (!yuv_file) {
		QMessageBox::information(this, "", "open yuv failed!");
		return;
	}
	ui.setupUi(this);
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


	startTimer(10);
}

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
	if (view->IsExit()) {
		view->Close();
		exit(0);
	}
	// yuv平面存储
	// yyyy yyyy uu vv
	//yuv_file.read((char *)yuv, sdl_width * sdl_height * 1.5);
	view->DrawFrame(frame);
	//view->Draw(yuv_1);
}

void sdlqtrgb::resizeEvent(QResizeEvent * ev)
{
	ui.label->resize(size());
	ui.label->move(0, 0);
	//view->Scale(width(), height());
}
