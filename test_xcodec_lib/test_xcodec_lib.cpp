#include <iostream>
#include "xdemuxtask.h"
using namespace std;
int main(int argc, char* argv[])
{
	cout << "test xcodec lib" << endl;
	XDemuxTask demux_task;
	demux_task.Open("v1080.mp4");
	demux_task.Start();
	getchar();
	return 0;
}