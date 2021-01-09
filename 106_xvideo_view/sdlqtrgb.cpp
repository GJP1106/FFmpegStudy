#include "sdlqtrgb.h"
//#include <sdl/SDL.h>
#include <QtWidgets/QMessageBox>
#include <iostream>
#include <fstream>
#include "xvideo_view.h"
using namespace std;

#pragma comment(lib, "SDL2.lib")
//static SDL_Window* sdl_win = NULL;
//static SDL_Renderer* sdl_render = NULL;
//static SDL_Texture* sdl_textture = NULL;
static int sdl_width = 0;
static int sdl_height = 0;
static unsigned char *yuv = NULL;
static unsigned char *yuv_1 = NULL;
static int pix_size = 2;
static ifstream yuv_file;
static XVideoView* view = nullptr;

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

	yuv = new unsigned char[sdl_width * sdl_height * 4];
	yuv_1 = new unsigned char[sdl_width * sdl_height * 4];
	startTimer(10);
}

void sdlqtrgb::timerEvent(QTimerEvent * ev)
{
	if (view->IsExit()) {
		view->Close();
		exit(0);
	}
	// yuv平面存储
	// yyyy yyyy uu vv
	yuv_file.read((char *)yuv, sdl_width * sdl_height * 1.5);
	//yuv_file.read((char *)yuv, sdl_width * sdl_height * 1.5);
	//QMessageBox::information(this, "", "open yuv failed!");
	int offset = 0;
	int i, j;
	for (i = 0; i < 500; i++) {
		//yuv += (i * 4) + 300;
		memcpy(yuv_1 + (i * 400), yuv + (i * 400), 400 * sizeof(unsigned char));
		/*for (j = 0; j < sdl_width; j++) {
			yuv_1[i * sdl_width + j] = yuv[i * sdl_width + j];
		}*/
	}
	offset = 400 * 500;
	/*for (j = 0; j < 150; j++) {
		memcpy(yuv_1 + offset + (j * 400), yuv + offset + (j * 400), 400 * sizeof(unsigned char));
	}
	offset += j * sdl_width;*/
	for (i = 0; i < 500; i++) {
		//yuv += (i * 4) + 300;
		memcpy(yuv_1 + offset + (i * 400), yuv + (i * 400), 400 * sizeof(unsigned char));
		/*for (j = 0; j < sdl_width; j++) {
			yuv_1[i * sdl_width + j + offset] = yuv[i * sdl_width + j];
		}*/
	}

	offset += 400 * i;
	/*for (j = 0; j < 150; j++) {
		memcpy(yuv_1 + offset + (j * 400), yuv + offset + (j * 400), 400 * sizeof(unsigned char));
	}
	offset += j * sdl_width;*/
	for (i = 0; i < 300; i++) {
		//yuv += (i * 4) + 300;
		//memcpy(yuv_1 + (i * 400), yuv + (i * 400), 400 * sizeof(unsigned char));
		for (j = 0; j < sdl_width; j++) {
			//yuv_1[i * sdl_width + j + offset] = yuv[i * sdl_width + j];
		}
	}

	offset += 400 * i;
	/*for (j = 0; j < 150; j++) {
		memcpy(yuv_1 + offset + (j * 400), yuv + offset + (j * 400), 400 * sizeof(unsigned char));
	}
	offset += j * sdl_width;*/
	for (i = 0; i < 300; i++) {
		//yuv += (i * 4) + 300;
		//memcpy(yuv_1 + (i * 400), yuv + (i * 400), 400 * sizeof(unsigned char));
		for (j = 0; j < sdl_width; j++) {
			//yuv_1[i * sdl_width + j + offset] = yuv[i * sdl_width + j];
		}
	}

	view->Draw(yuv_1);
}

void sdlqtrgb::resizeEvent(QResizeEvent * ev)
{
	ui.label->resize(size());
	ui.label->move(0, 0);
	//view->Scale(width(), height());
}
