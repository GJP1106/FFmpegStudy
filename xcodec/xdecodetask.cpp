#include "xdecodetask.h"
#include <iostream>
using namespace std;
extern "C"
{
#include <libavcodec/avcodec.h>
}
void XDecodeTask::Stop()
{
	pkt_list_.Clear();
	unique_lock<mutex> lock(mux_);
	decode_.set_c(nullptr);
	is_open_ = false;
	if (time_base_) {
		delete time_base_;
	}
	time_base_ = nullptr;
	while (!frames_.empty()) {
		av_frame_free(&frames_.front());
		frames_.pop_front();
	}
	XThread::Stop();
}
void XDecodeTask::set_time_base(AVRational * time_base)
{
	if (!time_base) return;
	unique_lock<mutex> lock(mux_);
	if (time_base_) {
		delete time_base_;
	}
	time_base_ = new AVRational();
	time_base_->den = time_base->den;
	time_base_->num = time_base->num;
}
// 打开解码器
bool XDecodeTask::Open(AVCodecParameters * para)
{
	if (!para) {
		LOGERROR("para is null!");
		return false;
	}
	unique_lock<mutex> lock(mux_);
	is_open_ = false;
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
	is_open_ = true;
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
	if (block_size_ <= 0) return;
	while (!is_exit_) {
		if (pkt_list_.Size() > block_size_) {
			MSleep(1);
			continue;
		}
		break;
	}
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
	long long cur_pts = -1;		//当前解码到的pts(以解码数据为准)
	while (!is_exit_) {

		// 同步
		while (!is_exit_) {
			if (syn_pts_ >= 0 && cur_pts > syn_pts_) {
				MSleep(1);
				continue;
			}
			break;
		}
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
				cur_pts = frame_->pts;
				// 转换成毫秒
				if (time_base_) {
					cur_ms_ = av_rescale_q(frame_->pts, *time_base_, { 1, 1000 });
				}
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


