#pragma once
#include <mutex>
#include <vector>
#include "xtools.h"
///////////////////////////////////////////
//// 编码和解码的基类
class XCODEC_API XCodec
{
public:
	////////////////////////////////////////////////
	/// 创建编码上下文
	/// @para codec_id 编码器ID号，对应ffmpeg
	/// @return 编解码上下文，失败返回nullptr
	static AVCodecContext* Create(int codec_id, bool is_encode);
	////////////////////////////////////////////////
	/// 设置对象的编码器上下文 上下文传递到对象中，资源由XEncode维护
	/// 加锁，线程安全
	/// @para c编码器上下文 如果c_不为nullptr，则先清理资源
	void set_c(AVCodecContext* c);

	//////////////////////////////////////////////
	/// 设置编码参数，线程安全
	bool SetOpt(const char* key, const char* val);
	bool SetOpt(const char* key, int val);

	/////////////////////////////////////////////////
	/// 打开编码器 线程安全
	bool Open();

	///////////////////////////////////////////////////////////////////////////////////
	/// 根据AVCodecContext 创建一个AVFrame， 需要调用者释放av_frame_free
	AVFrame* CreateFrame();
protected:
	AVCodecContext* c_ = nullptr;	//编码器上下文
	std::mutex mux_;				//编码器上下文锁
};

