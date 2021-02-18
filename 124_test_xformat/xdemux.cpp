#include "xdemux.h"
#include <iostream>
#include <thread>
using namespace std;

extern "C" {
#include <libavformat/avformat.h>
}
static void PrintErr(int err)
{
	char buf[1024] = { 0 };
	av_strerror(err, buf, sizeof(buf) - 1);
	cerr << buf << endl;
	return;
}
#define BERR(err) if (err != 0) {PrintErr(err); return 0;}
AVFormatContext * XDemux::Open(const char * url)
{
	AVFormatContext* c = nullptr;
	// �򿪷�װ������
	auto re = avformat_open_input(&c, url, nullptr, nullptr);
	BERR(re);
	// ��ȡý����Ϣ
	re = avformat_find_stream_info(c, NULL);
	BERR(re);
	// ��ӡ�����װ��Ϣ
	av_dump_format(c, 0, url, 0);
	return c;
}

bool XDemux::Read(AVPacket *pkt)
{
	unique_lock<mutex> lock(mux_);
	if (!c_) return false;
	auto re = av_read_frame(c_, pkt);
	BERR(re);
	return true;
}

bool XDemux::Seek(long long pts, int stream_index)
{
	unique_lock<mutex> lock(mux_);
	if (!c_) return false;
	auto re = av_seek_frame(c_, stream_index, pts,
		AVSEEK_FLAG_FRAME | AVSEEK_FLAG_BACKWARD);    //���ؼ�֡
	BERR(re);
	return true;
}
