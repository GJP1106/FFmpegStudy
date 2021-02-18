#pragma once
#include "xcodec.h"

class XEncode : public XCodec
{
public:
	////////////////////////////////////////////////
	/// 编码数据，线程安全 每次新创建AVPacket
	/// @para frame 空间由用户维护
	/// @return 失败返回nullptr，返回的AVPacket用户需要通过av_packet_free 清理
	AVPacket *Encode(const AVFrame* frame);

	///////////////////////////////////////////////////////////////
	//返回所有编码缓存中AVPacket
	std::vector<AVPacket *> End();
private:
	//AVCodecContext* c_ = nullptr;	//编码器上下文
	//std::mutex mux_;				//编码器上下文锁
};

