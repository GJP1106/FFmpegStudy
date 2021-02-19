#pragma once
#include "xtools.h"
#include "xmux.h"
class XCODEC_API XMuxTask : public XThread
{
public:
	void Main() override;
	// 接收数据
	void Do(AVPacket* pkt);
	bool Open(const char *url,
		AVCodecParameters *video_para = nullptr,
		AVRational *video_time_base = nullptr,
		AVCodecParameters *audio_para = nullptr,
		AVRational *audio_time_base = nullptr
		);

private:
	XMux xmux_;
	XAVPacketList pkts_;
	std::mutex mux_;
};

