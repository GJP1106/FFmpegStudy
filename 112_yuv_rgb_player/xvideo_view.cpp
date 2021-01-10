//#include "xvideo_view.h"
#include "xsdl.h"

extern "C"
{
#include <libavcodec/avcodec.h>
}

#pragma comment(lib, "avutil.lib")
using namespace std;
void MSleep(unsigned int ms)
{
	auto beg = clock();
	for (int i = 0; i < ms; i++) {
		this_thread::sleep_for(1ms);
		if ((clock() - beg) / (CLOCKS_PER_SEC / 1000) >= ms) {
			break;
		}

	}
}

XVideoView * XVideoView::Create(RenderType type)
{
	switch (type)
	{
	case XVideoView::SDL:
		return new XSDL();
	default:
		break;
	}
	return nullptr;
}

bool XVideoView::DrawFrame(AVFrame * frame)
{
	if (!frame || !frame->data[0]) return false;
	count_++;
	if (beg_ms <= 0) {
		beg_ms = clock();
	}
	// 计算显示帧率
	else if ((clock() - beg_ms) / (CLOCKS_PER_SEC / 1000) >= 1000) {  //一秒计算一次fps
		render_fps_ = count_;
		count_ = 0;
		beg_ms = clock();
	}

	switch (frame->format)
	{
	case AV_PIX_FMT_YUV420P:
		return Draw(frame->data[0], frame->linesize[0],
			frame->data[1], frame->linesize[1],
			frame->data[2], frame->linesize[2]);
	case AV_PIX_FMT_BGRA:
		return Draw(frame->data[0], frame->linesize[0]);
	default:
		break;
	}
	return true;
}

bool XVideoView::Open(std::string filepath)
{
	if (ifs_.is_open()) {
		ifs_.close();
	}
	ifs_.open(filepath, ios::binary);

	return ifs_.is_open();
}
