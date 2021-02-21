#include "xdemuxtask.h"
#include <iostream>
using namespace std;
extern "C" {
#include <libavformat/avformat.h>
}
using namespace std;
void XDemuxTask::Main()
{
	AVPacket pkt;
	while (!is_exit_) {
		if (!demux_.Read(&pkt)) {
			// 读取失败
			cout << "--" << endl;
			if (!demux_.is_connected()) {
				Open(url_, timeout_ms_);
			}
			this_thread::sleep_for(1ms);
			continue;
		}
		cout << "." << flush;
		// 播放速度控制
		if (syn_type_ == XSYN_VIDEO &&
			pkt.stream_index == demux_.video_index()) {
			//pkt.duration
			auto dur = demux_.RescaleToMs(pkt.duration, pkt.stream_index);
			if (dur <= 0) {
				dur = 40;
			}
			MSleep(dur);
		}
		Next(&pkt);
		av_packet_unref(&pkt);
		this_thread::sleep_for(1ms);
	}
}

bool XDemuxTask::Open(std::string url, int timeout_ms)
{
	LOGDEBUG("XDemuxTask::Open() begin!");
	demux_.set_c(nullptr);  //断开之后重连
	this->url_ = url;
	this->timeout_ms_ = timeout_ms;
	auto c = demux_.Open(url.c_str());
	if (!c) return false;
	demux_.set_c(c);
	demux_.set_time_out_ms(timeout_ms);
	LOGDEBUG("XDemuxTask::Open() end!");
	return true;
}

void XDemuxTask::Stop()
{
	XThread::Stop();
	demux_.set_c(nullptr);
}
