#pragma once
#include "xtools.h"
#include "xdemuxtask.h"
#include "xdecodetask.h"
#include "xvideo_view.h"
class XCODEC_API XPlayer : public XThread
{
public:
	// 回调接收音视频包
	void Do(AVPacket* pkt) override;
	// 打开音视频 初始花播放和渲染
	bool Open(const char* url, void* winid);
	void Stop();
	// 主线程 处理同步
	void Main() override;
	// 开启 解封装 音视频解码 和 处理同步的线程
	void Start();
	// 渲染视频 播放音频
	void Update();
protected:
	XDemuxTask demux_;				//解封装
	XDecodeTask audio_decode_;		//音频解码
	XDecodeTask video_decode_;		//视频解码
	XVideoView* view_ = nullptr;	//视频渲染
};

