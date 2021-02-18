#pragma once
#include "xformat.h"
class XDemux : public XFormat
{
public:
	/* 打开解封装
	   解封装地址支持rtsp
	*/
	static AVFormatContext* Open(const char* url);

	// 读取一帧数据
	bool Read(AVPacket *pkt);

	bool Seek(long long pts, int stream_index);
};

