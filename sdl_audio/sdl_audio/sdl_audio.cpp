// sdl_audio.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
//#include <sdl/SDL.h>
#include <fstream>
#include "xaudio_play.h"

#pragma comment(lib, "SDL2.lib")
#undef main
using namespace std;

/*void AudioCallback(void* userdata, Uint8* stream, int len)
{
	cout << "AudioCallback" << endl;
	SDL_memset(stream, 0, len);
	auto ifs = (ifstream *)userdata;
	ifs->read((char *)stream, len);
	if (ifs->gcount() <= 0) {
		cout << "end" << endl;
		SDL_PauseAudio(1);
	}
}*/

int main(int argc, char* argv[])
{
#if 0
    // 初始化SDL 音频模块
	SDL_Init(SDL_INIT_AUDIO);

	// 打开音频设备
	SDL_AudioSpec spec;

	spec.freq = 44100;		//音频一秒采样率
	spec.format = AUDIO_S16SYS;	//音频样本类型，使用系统字节序
	spec.channels = 2;			//音频通道数量(双声道 立体声)
	spec.silence = 0;			//静音的值
	spec.samples = 1024;		//样本数量，2的n次方，用于平面模式的多通道数据，例如：sample = 8 LLLL RRRR
	spec.callback = AudioCallback;	//音频播放回调
	ifstream ifs("test_pcm.pcm", ios::binary);	//二进制打开
	if (!ifs) {
		cerr << "open pcm file failed!" << endl;
		return -1;
	}
	spec.userdata = &ifs;

	if (SDL_OpenAudio(&spec, nullptr) < 0) {
		cerr << "SDL_OpenAudio failed" << SDL_GetError() << endl;
	}
	SDL_PauseAudio(0);
	getchar();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);
#endif


	auto audio = XAudioPlay::Instace();
	XAudioSpec spec;
	spec.freq = 44100;
	audio->Open(spec);
	ifstream ifs("test_pcm.pcm", ios::binary);
	if (!ifs) return -1;
	unsigned char buf[1024] = { 0 };
	audio->set_volume(10);
	for (;;) {
		ifs.read((char*)buf, sizeof(buf));
		int len = ifs.gcount();
		if (len <= 0) break;
		audio->Push(buf, len);
	}
	getchar();
	audio->SetSpeed(0.5);
	getchar();
	audio->SetSpeed(2);
	getchar();
	audio->Close();
	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
