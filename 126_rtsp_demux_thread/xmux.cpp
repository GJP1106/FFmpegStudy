#include "xmux.h"
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
/// �򿪷�װ
AVFormatContext * XMux::Open(const char * url)
{
	AVFormatContext *c = nullptr;
	// ����������
	auto re = avformat_alloc_output_context2(&c, NULL, NULL, url);
	BERR(re);

	// �����Ƶ��Ƶ��
	auto vs = avformat_new_stream(c, NULL);		// ��Ƶ��
	vs->codecpar->codec_type = AVMEDIA_TYPE_VIDEO;
	auto as = avformat_new_stream(c, NULL);		// ��Ƶ��
	as->codecpar->codec_type = AVMEDIA_TYPE_AUDIO;

	// ��IO
	re = avio_open(&c->pb, url, AVIO_FLAG_WRITE);
	BERR(re);

	return c;
}

bool XMux::WriteHead()
{
	unique_lock<mutex> lock(mux_);
	if (!c_) return false;
	auto re = avformat_write_header(c_, NULL);
	BERR(re);
	// ��ӡ���������
	av_dump_format(c_, 0, c_->url, 1);
	return true;
}

bool XMux::Write(AVPacket * pkt)
{
	unique_lock<mutex> lock(mux_);
	if (!c_) return false;
	// д��һ֡���ݣ� �ڲ���������DTS��ͨ��pkt=null ����д�뻺��
	auto re = av_interleaved_write_frame(c_, pkt);
	BERR(re);
	return true;
}

bool XMux::WriteEnd()
{
	unique_lock<mutex> lock(mux_);
	if (!c_) return false;
	av_interleaved_write_frame(c_, NULL);  //д�����򻺳�
	auto re = av_write_trailer(c_);
	BERR(re);
	return true;
}
