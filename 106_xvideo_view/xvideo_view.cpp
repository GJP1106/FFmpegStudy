//#include "xvideo_view.h"
#include "xsdl.h"
extern "C"
{
#include <libavcodec/avcodec.h>
}

#pragma comment(lib, "avutil.lib")

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
