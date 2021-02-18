#pragma once
// 封装和解封装基类
#include <mutex>
struct AVFormatContext;
struct AVCodecParameters;
struct AVCodecContext;
struct AVPacket;
struct XRational {
	int num;
	int den;
};
class XFormat
{
public:
	// 复制参数， 线程安全
	//
	bool CopyPara(int stream_index, AVCodecParameters* dst);
	bool CopyPara(int stream_index, AVCodecContext* dst);
	// 设置上下文， 并且清理上次的设置的值，如果传递NULL,  相当于关闭上下文
	// 线程安全
	void set_c(AVFormatContext *c);
	int audio_index() { return audio_index_; }
	int video_index() { return video_index_; }
	XRational video_time_base() { return video_time_base_; }
	XRational audio_time_base() { return audio_time_base_; }

	// 根据timebase换算时间
	bool RescaleTime(AVPacket *pkt, long long offset_pts, XRational time_base);
	int video_codec_id() { return video_codec_id_; }
protected:
	AVFormatContext* c_ = nullptr;		// 封装和解封装上下文
	std::mutex mux_;					// c_资源互斥
	int video_index_ = 0;				//video和audio在stream中的索引
	int audio_index_ = 1;
	XRational video_time_base_ = {1, 25};
	XRational audio_time_base_ = { 1, 9000 };
	int video_codec_id_ = 0;                   // 编码器ID
};

