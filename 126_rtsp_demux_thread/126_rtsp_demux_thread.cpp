#include <iostream>
#include <thread>
#include "xtools.h"
#include "xdemuxtask.h"
#include "xdecodetask.h"
#include "xvideo_view.h"

using namespace std;
//class TestThread : public XThread
//{
//public:
//	void Main()
//	{
//		LOGDEBUG("TestTHread Main begin");
//		while (!is_exit_) {
//			this_thread::sleep_for(1ms);
//		}
//		LOGDEBUG("TestTHread Main end");
//	}
//};
//#define CAM1 "rtsp://127.0.0.1:8554/test"
//#define CAM1 "rtsp://3.84.6.190/vod/mp4:BigBuckBunny_115k.mov"

#define CAM1 "400_300_25.mp4"
int main(int argc, char* argv[])
{
	//TestThread tt;
	//tt.Start();
	//this_thread::sleep_for(3s);
	//tt.Stop();
	XDemuxTask det;
	for (;;) {
		if (det.Open(CAM1)) {
			break;
		}
		MSleep(100);
		continue;
	}
	auto para = det.CopyVideoPara();
	// 初始化渲染
	auto view = XVideoView::Create();
	view->Init(para->para);
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

	getchar();
	return 0;
}