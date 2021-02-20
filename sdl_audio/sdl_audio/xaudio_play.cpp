#include "xaudio_play.h"
#include <iostream>
#include <sdl/SDL.h>

#pragma comment(lib, "SDL2.lib")
using namespace std;


class CXAudioPlay : public XAudioPlay
{
public:
	bool Open(XAudioSpec& spec) {
		this->spec_ = spec;
		// �˳���һ����Ƶ
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
		// ��ʼ����
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
		// 1��buf����stream���� offset��¼λ��
		// 2��bufС��stream���� ƴ��
		int mixed_size = 0;		//�Ѿ�������ֽ���
		int need_size = len;	//��Ҫ������ֽ���
		while (mixed_size < len) {
			if (audio_datas_.empty()) break;
			buf = audio_datas_.front();
			int size = buf.data.size() - buf.offset;		//ʣ��δ���������
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

XAudioPlay::XAudioPlay()
{
	SDL_Init(SDL_INIT_AUDIO);
}
