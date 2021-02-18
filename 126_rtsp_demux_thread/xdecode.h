#pragma once
#include "xcodec.h"
struct AVBufferRef;
class XDecode : public XCodec
{
public:
	bool Send(const AVPacket* pkt);  //发送编码
	bool Recv(AVFrame *frame, bool is_hw_type = false);  //获取解码
	std::vector<AVFrame*> End();  // 获取缓存

	////////////////////////////////////////////////
	//// 初始化硬件加速 4 AV_HWDEVICE_TYPE_DXVA2
	bool InitHW(int type = 4);
private:
	AVBufferRef *hw_device_ctx = nullptr;
};

