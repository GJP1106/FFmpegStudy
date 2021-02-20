#pragma once
#include "xtools.h"
#include "xdemuxtask.h"
#include "xdecodetask.h"
#include "xvideo_view.h"
class XCODEC_API XPlayer : public XThread
{
public:
	void Do(AVPacket* pkt) override;
	bool Open(const char* url, void* winid);
	void Main() override;
	void Start();
protected:
	XDemuxTask demux_;				//解封装
	XDecodeTask audio_decode_;		//音频解码
	XDecodeTask video_decode_;		//视频解码
	XVideoView* view_ = nullptr;	//视频渲染
};

