#include <iostream>
#include <thread>
#include "xtools.h"
#include "xdemuxtask.h"

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
#define CAM1 "rtsp://127.0.0.1:8554/test"
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
	det.Start();
	getchar();
	return 0;
}