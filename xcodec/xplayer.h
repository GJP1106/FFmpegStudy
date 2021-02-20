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
	XDemuxTask demux_;				//���װ
	XDecodeTask audio_decode_;		//��Ƶ����
	XDecodeTask video_decode_;		//��Ƶ����
	XVideoView* view_ = nullptr;	//��Ƶ��Ⱦ
};

