#include <iostream>
#include "xaudio_play.h"
//#include "xdemuxtask.h"
//#include "xdecodetask.h"
#include "xplayer.h"
using namespace std;
int main(int argc, char* argv[])
{
	XPlayer player;
	player.Open("400_300_25.mp4", nullptr);
	player.Start();
	getchar();
#if 0
	// ���װ
	XDemuxTask demux;
	if (!demux.Open("v1080.mp4")) {
		cerr << "demux open failed!" << endl;
		return -1;
	}

	auto ap = demux.CopyAudioPara();
	if (!ap) {
		cerr << "no audio" << endl;
		return -2;
	}

	//����
	XDecodeTask decode;
	if (!decode.Open(ap->para)) {
		cerr << "decode open failed !" << endl;
		return -3;
	}
	decode.set_stream_index(demux.audio_index());
	demux.set_next(&decode);
	demux.Start();
	decode.Start();
	// ��Ƶ����
	auto audio = XAudioPlay::Instace();
	if (!audio->Open(ap->para)) {
		cerr << "audio open failed!" << endl;
		return -4;
	}
	decode.set_frame_cache(true);
	for (;;) {
		auto f = decode.GetFrame();
		if (!f) {
			MSleep(10);
			continue;
		}
		audio->Push(f);
		XFreeFrame(&f);
	}

	getchar();
#endif

	return 0;
}