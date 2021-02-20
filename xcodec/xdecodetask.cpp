#include "xdecodetask.h"
#include <iostream>
using namespace std;
extern "C"
{
#include <libavcodec/avcodec.h>
}
// 打开解码器
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
	// 复制视频参数
	avcodec_parameters_to_context(c, para);
	decode_.set_c(c);
	if (!decode_.Open()) {
		LOGERROR("decode_.Open() failed!");
		return false;
	}
	LOGINFO("Open decode success!");
	return true;
}

// 责任链处理函数
void XDecodeTask::Do(AVPacket * pkt)
{
	cout << "#" << flush;
	if (!pkt || pkt->stream_index != stream_index_) { //判断是否是视频
		return;
	}
	pkt_list_.Push(pkt);
}

// 线程主函数
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
		// 发送到解码线程
		bool re = decode_.Send(pkt);
		av_packet_free(&pkt);
		if (!re) {
			this_thread::sleep_for(1ms);
			continue;
		}
		{
			unique_lock<mutex> lock(mux_);
			if (decode_.Recv(frame_)) {
				need_view_ = true;
				cout << "@" << flush;
			}
			if (frame_cache) {
				auto f = av_frame_alloc();
				av_frame_ref(f, frame_);	//引用计数+1
				frames_.push_back(f);
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

AVFrame * XDecodeTask::GetFrame()
{
	unique_lock<mutex> lock(mux_);
	if (frame_cache) {
		if (frames_.empty()) return nullptr;
		auto f = frames_.front();
		frames_.pop_front();
		return f;
	}
	if (!need_view_ || !frame_ || !frame_->buf[0]) return nullptr;
	auto f = av_frame_alloc();
	auto re = av_frame_ref(f, frame_);	//引用加1
	if (re != 0) {
		av_frame_free(&f);
		PrintErr(re);
		return nullptr;
	}
	need_view_ = false;

	return f;
}
