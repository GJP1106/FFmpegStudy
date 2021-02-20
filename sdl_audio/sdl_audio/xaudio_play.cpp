#include "xaudio_play.h"
#include <iostream>
#include <sdl/SDL.h>

#pragma comment(lib, "SDL2.lib")
using namespace std;


class CXAudioPlay : public XAudioPlay
{
public:
	bool Open(XAudioSpec& spec) {
		// 退出上一次音频
		SDL_QuitSubSystem(SDL_INIT_AUDIO);

		SDL_AudioSpec sdl_spec;
		sdl_spec.freq = spec.freq;
		sdl_spec.format = spec.format;
		sdl_spec.channels = spec.channels;
		sdl_spec.samples = spec.samples;
		sdl_spec.silence = 0;
		sdl_spec.userdata = this;
		sdl_spec.callback = AudioCallback;
		if (SDL_OpenAudio(&sdl_spec, nullptr) < 0) {
			cerr << "SDL_OpenAudio fail" << SDL_GetError() << endl;
			return false;
		}
		// 开始播放
		SDL_PauseAudio(0);
		return true;
	}
	void Callback(unsigned char* stream, int len)
	{
		SDL_memset(stream, 0, len);
		unique_lock<mutex> lock(mux_);
		if (audio_datas_.empty()) return;
		auto buf = audio_datas_.front();
		// 1、buf大于stream缓冲 offset记录位置
		// 2、buf小于stream缓冲 拼接
		int mixed_size = 0;		//已经处理的字节数
		int need_size = len;	//需要处理的字节数
		while (mixed_size < len) {
			if (audio_datas_.empty()) break;
			buf = audio_datas_.front();
			int size = buf.data.size() - buf.offset;		//剩余未处理的数据
			if (size > need_size) {
				size = need_size;
			}
			SDL_MixAudio(stream + mixed_size,
				buf.data.data() + buf.offset,
				size, SDL_MIX_MAXVOLUME);
			need_size -= size;
			mixed_size += size;
			buf.offset += size;
			if (buf.offset >= buf.data.size()) {
				audio_datas_.pop_front();
			}
		}
	}
};

XAudioPlay * XAudioPlay::Instace()
{
	static CXAudioPlay cx;
	return &cx;
}

XAudioPlay::XAudioPlay()
{
	SDL_Init(SDL_INIT_AUDIO);
}
