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
		if (demux_.Read(&pkt)) {
			// 读取失败
			cout << "--" << endl;
			if (!demux_.is_connected()) {
				Open(url_, timeout_ms_);
			}
			this_thread::sleep_for(1ms);
			continue;
		}
		cout << "." << flush;
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
