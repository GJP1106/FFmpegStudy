#include "xdecodetask.h"
#include <iostream>
using namespace std;
extern "C"
{
#include <libavcodec/avcodec.h>
}
// �򿪽�����
bool XDecodeTask::Open(AVCodecParameters * para)
{
	if (!para) {
		LOGERROR("para is null!");
		return false;
	}
	unique_lock<mutex> lock(mux_);
	auto c = decode_.Create(para->codec_id, false);
	if (!c) {
		LOGERROR("decode_.Create failed!");
		return false;
	}
	// ������Ƶ����
	avcodec_parameters_to_context(c, para);
	decode_.set_c(c);
	if (!decode_.Open()) {
		LOGERROR("decode_.Open() failed!");
		return false;
	}
	LOGINFO("Open decode success!");
	return true;
}

// ������������
void XDecodeTask::Do(AVPacket * pkt)
{
	cout << "#" << flush;
	if (!pkt || pkt->stream_index != 0) { //�ж��Ƿ�����Ƶ
		return;
	}
	pkt_list_.Push(pkt);
}

// �߳�������
void XDecodeTask::Main()
{
	{
		unique_lock<mutex> lock(mux_);
	    if (!frame_) {
		    frame_ = av_frame_alloc();
	    }
	}
	while (!is_exit_) {
		auto pkt = pkt_list_.Pop();
		if (!pkt) {
			this_thread::sleep_for(1ms);
			continue;
		}
		// ���͵������߳�
		bool re = decode_.Send(pkt);
		av_packet_free(&pkt);
		if (!re) {
			this_thread::sleep_for(1ms);
			continue;
		}
		{
			unique_lock<mutex> lock(mux_);
			if (decode_.Recv(frame_)) {
				cout << "@" << flush;
			}
		}
		this_thread::sleep_for(1ms);
	}
	{
		unique_lock<mutex> lock(mux_);
		if (frame_) {
			av_frame_free(&frame_);
		}
	}
}
