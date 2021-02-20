#include "xaudio_play.h"
#include <iostream>
#include <sdl/SDL.h>
extern "C"
{
#include <libavformat/avformat.h>
}

//#pragma comment(lib, "SDL2.lib")
using namespace std;


class CXAudioPlay : public XAudioPlay
{
public:
	bool Open(XAudioSpec& spec) {
		this->spec_ = spec;
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
	void Close()
	{
		SDL_QuitSubSystem(SDL_INIT_AUDIO);
		unique_lock<mutex> lock(mux_);
		audio_datas_.clear();
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
				size, volume_);
			need_size -= size;
			mixed_size += size;
			buf.offset += size;
			if (buf.offset >= buf.data.size()) {
				audio_datas_.pop_front();
			}
		}
	}
private:

};

XAudioPlay * XAudioPlay::Instace()
{
	static CXAudioPlay cx;
	return &cx;
}

bool XAudioPlay::Open(AVCodecParameters * para)
{
	XAudioSpec spec;
	spec.channels = para->channels;
	spec.freq = para->sample_rate;
	AVSampleFormat;
	switch (para->format)
	{
	case AV_SAMPLE_FMT_S16:
	case AV_SAMPLE_FMT_S16P:
		spec.format = AUDIO_S16;
		break;
	case AV_SAMPLE_FMT_S32:
	case AV_SAMPLE_FMT_S32P:
		spec.format = AUDIO_S32;
		break;
	case AV_SAMPLE_FMT_FLT:
	case AV_SAMPLE_FMT_FLTP:
		spec.format = AUDIO_F32;
		break;
	default:
		break;
	}
	return Open(spec);
}

void XAudioPlay::Push(AVFrame * frame)
{
	if (!frame || !frame->data[0]) return;
	vector<unsigned char> buf;
	int sample_size = 4;
	int channels = frame->channels;
	unsigned char *L = frame->data[0];
	unsigned char *R = frame->data[1];
	unsigned char* data = nullptr;
	// 暂时支持双通道
	switch (frame->format)
	{
	//case AV_SAMPLE_FMT_S16P:
	case AV_SAMPLE_FMT_S32P:
	case AV_SAMPLE_FMT_FLTP:
		buf.resize(frame->linesize[0]);
		data = buf.data();
		// LLLL RRRR
		// LR LR LR

		for (int i = 0; i < frame->nb_samples; i++) {
			memcpy(data + i * sample_size * channels, L + i * sample_size, sample_size);
			memcpy(data + i * sample_size * channels + sample_size,
				R + i * sample_size, sample_size);
		}
		Push(data, frame->linesize[0]);
		return;
		break;
	default:
		break;
	}
	Push(frame->data[0], frame->linesize[0]);
}

XAudioPlay::XAudioPlay()
{
	SDL_Init(SDL_INIT_AUDIO);
}
