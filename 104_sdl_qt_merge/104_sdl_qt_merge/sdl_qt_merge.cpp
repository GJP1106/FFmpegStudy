#include "sdl_qt_merge.h"
#include <sdl/SDL.h>
#include <QtWidgets/QMessageBox>
#include <iostream>
using namespace std;
#pragma comment(lib, "SDL2.lib")
static SDL_Window* sdl_win = NULL;
static SDL_Renderer* sdl_render = NULL;
static SDL_Texture* sdl_textture = NULL;
static int sdl_width = 0;
static int sdl_height = 0;
static unsigned char *rgb = NULL;
static int pix_size = 4;


sdl_qt_merge::sdl_qt_merge(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	sdl_width = ui.label->width();
	sdl_height = ui.label->height();
	SDL_Init(SDL_INIT_VIDEO);
	sdl_win = SDL_CreateWindowFrom((void*)ui.label->winId());
	if (!sdl_win) {
		cout << SDL_GetError() << endl;
	}
	sdl_render = SDL_CreateRenderer(sdl_win, -1, SDL_RENDERER_ACCELERATED);
	if (!sdl_render) {
		cout << SDL_GetError() << endl;
	}

	QImage img1("001.png");
	QImage img2("002.png");
	if (img1.isNull() || img2.isNull()) {
		QMessageBox::information(this, "", "open image failed!");
		return;
	}

	int out_w = img1.width() + img2.width();
	int out_h = img1.height();
	if (out_h < img2.height()) out_h = img2.height();

	sdl_width = out_w;
	sdl_height = out_h;
	resize(sdl_width, sdl_height);
	ui.label->move(0, 0);
	ui.label->resize(sdl_width, sdl_height);

	sdl_textture = SDL_CreateTexture(sdl_render,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		sdl_width,
		sdl_height);

	rgb = new unsigned char[sdl_width * sdl_height * 4];
	// 默认设置为透明
	memset(rgb, 0, sdl_width * sdl_height * pix_size);
	// 合并两幅图像
	for (int i = 0; i < sdl_height; i++) {
		int b = i * sdl_width * pix_size;
		if (i < img1.height()) {
			memcpy(rgb + b, img1.scanLine(i), img1.width() * pix_size);
		}
		b += img1.width() * pix_size;
		if (i < img2.height()) {
			memcpy(rgb + b, img2.scanLine(i), img2.width() * pix_size);
		}
	}
	QImage out(rgb, sdl_width, sdl_height, QImage::Format_ARGB32);
	out.save("out.png");
	startTimer(10);

}

void sdl_qt_merge::timerEvent(QTimerEvent * ev)
{
	/*static unsigned char tmp = 255;
	tmp--;
	for (int j = 0; j < sdl_height; j++) {
		int b = j * sdl_width * pix_size;
		for (int i = 0; i < sdl_width * pix_size; i += pix_size) {
			//rgb[b + i] = 0; // B
			//rgb[b + i + 1] = tmp; // G
			//rgb[b + i + 2] = 0;  // R
			//rgb[b + i + 3] = 0;  // A
		}
	}*/
	SDL_UpdateTexture(sdl_textture, NULL, rgb, sdl_width * pix_size);
	SDL_RenderClear(sdl_render);
	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.h = sdl_height;
	rect.w = sdl_width;
	SDL_RenderCopy(sdl_render, sdl_textture, NULL, &rect);
	SDL_RenderPresent(sdl_render);
}
