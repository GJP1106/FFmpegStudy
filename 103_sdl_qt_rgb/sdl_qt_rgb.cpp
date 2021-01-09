#include "sdl_qt_rgb.h"
#include <sdl/SDL.h>
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

Sdl_Qt_Rgb::Sdl_Qt_Rgb(QWidget *parent)
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
	sdl_textture = SDL_CreateTexture(sdl_render,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		sdl_width,
		sdl_height);

	rgb = new unsigned char[sdl_width * sdl_height * 4];
	startTimer(10);
}

void Sdl_Qt_Rgb::timerEvent(QTimerEvent * ev)
{
	static unsigned char tmp = 255;
	tmp--;
	for (int j = 0; j < sdl_height; j++) {
		int b = j * sdl_width * pix_size;
		for (int i = 0; i < sdl_width * pix_size; i += pix_size) {
			rgb[b + i] = 0; // B
			rgb[b + i + 1] = tmp; // G
			rgb[b + i + 2] = 0;  // R
			rgb[b + i + 3] = 0;  // A
		}
	}
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
