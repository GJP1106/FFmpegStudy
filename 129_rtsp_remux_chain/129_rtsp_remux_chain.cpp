#include <iostream>
#include <thread>
#include "xtools.h"
#include "xdemuxtask.h"
#include "xdecodetask.h"
#include "xvideo_view.h"
#include "xmuxtask.h"

using namespace std;

//#define CAM1 "rtsp://127.0.0.1:8554/test"
//#define CAM1 "rtsp://3.84.6.190/vod/mp4:BigBuckBunny_115k.mov"

#define CAM1 "400_300_25.mp4"
int main(int argc, char* argv[])
{
	XDemuxTask demux_task;
	for (;;) {
		if (demux_task.Open(CAM1)) {
			break;
		}
		MSleep(100);
		continue;
	}
	auto vpara = demux_task.CopyVideoPara();
	AVCodecParameters* video_para = nullptr;
	AVCodecParameters* audio_para = nullptr;
	AVRational* video_time_base = nullptr;
	AVRational* audio_time_base = nullptr;
	if (vpara) {
		video_para = vpara->para;
		video_time_base = vpara->time_base;
	}
	auto apara = demux_task.CopyAudioPara();
	if (apara) {
		audio_para = apara->para;
		audio_time_base = apara->time_base;
	}

	XMuxTask mux_task;
	if (!mux_task.Open("rtsp_out.mp4", video_para, video_time_base, audio_para, audio_time_base)) {
		LOGERROR("mux_task.Open failed!");
		return -1;
	}
	demux_task.set_next(&mux_task);
	demux_task.Start();
	mux_task.Start();
	MSleep(3000);
	mux_task.Stop();

	if (!mux_task.Open("rtsp_out1.mp4", video_para, video_time_base, audio_para, audio_time_base)) {
		LOGERROR("mux_task.Open failed!");
		return -1;
	}
	mux_task.Start();
	MSleep(3000);
	mux_task.Stop();
#if 0
	if (vpara)
	// 初始化渲染
	auto view = XVideoView::Create();
	view->Init(vpara->para);
	XDecodeTask decode_task;
	if (!decode_task.Open(para->para)) {
		LOGERROR("open decode failed!");
	}
	else {
		// 设定下一个责任
		det.set_next(&decode_task);
		det.Start();
		decode_task.Start();
	}

	for (;;) {
		auto f = decode_task.GetFrame();
		if (!f) {
			MSleep(1);
			continue;
		}
		view->DrawFrame(f);
		XFreeFrame(&f);
	}
#endif
	getchar();
	return 0;
}