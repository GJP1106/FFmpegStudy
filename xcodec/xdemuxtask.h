#pragma once
#include "xtools.h"
#include "xdemux.h"

enum XSYN_TYPE
{
	XSYN_NONE = 0,	//不做同步
	XSYN_VIDEO = 1,	//根据视频同步， 不处理音频
};

class XCODEC_API XDemuxTask : public XThread
{
public:
    
	bool Seek(long long ms);
	int audio_index() { return demux_.audio_index(); }
	int video_index() { return demux_.video_index(); }
	void Main();
	bool Open(std::string url, int timeout_ms = 1000);
	// 复制视频参数
	std::shared_ptr<XPara> CopyVideoPara()
	{
		return demux_.CopyVideoPara();
	}
	std::shared_ptr<XPara> CopyAudioPara()
	{
		return demux_.CopyAudioPara();
	}
	void Stop();
	void set_syn_type(XSYN_TYPE t) { syn_type_ = t; }
private:
	XDemux demux_;
	std::string url_;
	int timeout_ms_ = 0;		//超时时间
	XSYN_TYPE syn_type_ = XSYN_NONE;
};

