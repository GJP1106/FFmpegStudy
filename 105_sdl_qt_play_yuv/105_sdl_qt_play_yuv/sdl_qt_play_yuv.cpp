#include "sdl_qt_play_yuv.h"
#include <sdl/SDL.h>
#include <QtWidgets/QMessageBox>
#include <iostream>
#include <fstream>
using namespace std;

#pragma comment(lib, "SDL2.lib")
static SDL_Window* sdl_win = NULL;
static SDL_Renderer* sdl_render = NULL;
static SDL_Texture* sdl_textture = NULL;
static int sdl_width = 0;
static int sdl_height = 0;
static unsigned char *yuv = NULL;
static int pix_size = 2;
static ifstream yuv_file;

sdl_qt_play_yuv::sdl_qt_play_yuv(QWidget *parent)
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
	SDL_Init(SDL_INIT_VIDEO);
	sdl_win = SDL_CreateWindowFrom((void*)ui.label->winId());
	if (!sdl_win) {
		cout << SDL_GetError() << endl;
	}
	sdl_render = SDL_CreateRenderer(sdl_win, -1, SDL_RENDERER_ACCELERATED);
	if (!sdl_render) {
		cout << SDL_GetError() << endl;
	}
	//  创建材质，支持YUV
	sdl_textture = SDL_CreateTexture(sdl_render,
		SDL_PIXELFORMAT_IYUV,
		SDL_TEXTUREACCESS_STREAMING,
		sdl_width,
		sdl_height);

	yuv = new unsigned char[sdl_width * sdl_height * 4];
	startTimer(10);
}

void sdl_qt_play_yuv::timerEvent(QTimerEvent * ev)
{
	/*static unsigned char tmp = 255;
	tmp--;
	for (int j = 0; j < sdl_height; j++) {
		int b = j * sdl_width * pix_size;
		for (int i = 0; i < sdl_width * pix_size; i += pix_size) {
			yuv[b + i] = 0; // B
			yuv[b + i + 1] = tmp; // G
			yuv[b + i + 2] = 0;  // R
			yuv[b + i + 3] = 0;  // A
		}
	}*/
	// yuv平面存储
	// yyyy yyyy uu vv
	yuv_file.read((char *)yuv, sdl_width * sdl_height * 1.5);
	SDL_UpdateTexture(sdl_textture, NULL, yuv,
		sdl_width //一行y的字节数
	);
	SDL_RenderClear(sdl_render);
	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.h = sdl_height;
	rect.w = sdl_width;
	SDL_RenderCopy(sdl_render, sdl_textture, NULL, &rect);
	SDL_RenderPresent(sdl_render);
}
