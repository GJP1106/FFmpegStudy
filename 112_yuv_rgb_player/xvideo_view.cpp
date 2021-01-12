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

long long NowMs()
{
	return clock() / (CLOCKS_PER_SEC / 1000);
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
	// ������ʾ֡��
	else if ((clock() - beg_ms) / (CLOCKS_PER_SEC / 1000) >= 1000) {  //һ�����һ��fps
		render_fps_ = count_;
		count_ = 0;
		beg_ms = clock();
	}

	switch (frame->format)
	{
	case AV_PIX_FMT_YUV420P:
		//cout << "AV_PIX_FMT_YUV420P" << endl;
		return Draw(frame->data[0], frame->linesize[0],
			frame->data[1], frame->linesize[1],
			frame->data[2], frame->linesize[2]);
	case AV_PIX_FMT_RGBA:
	case AV_PIX_FMT_ARGB:
	case AV_PIX_FMT_BGRA:
		return Draw(frame->data[0], frame->linesize[0]);
	case AV_PIX_FMT_RGB24:
		return Draw(frame->data[0], frame->linesize[0]);
	default:
		break;
	}
	cout << frame->format << endl;
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

AVFrame * XVideoView::Read()
{
	if (width_ <= 0 || height_ <= 0 || !ifs_) return NULL;
	// AVFrame �ռ��Ѿ�����, ������������仯����Ҫ�ͷſռ�
	if (frame_) {
		if (frame_->width != width_ ||
			frame_->height != height_ ||
			frame_->format != fmt_) {
			// �ͷ�AVFrame����ռ䣬��buf���ü�����һ
			av_frame_free(&frame_);
		}
	}
	if (!frame_) {
		// �������ռ�����ؿռ�
		frame_ = av_frame_alloc();
		frame_->width = width_;
		frame_->height = height_;
		frame_->format = fmt_;
		frame_->linesize[0] = width_ * 4;
		if (frame_->format == AV_PIX_FMT_YUV420P) {
			frame_->linesize[0] = width_;		//Y
			frame_->linesize[1] = width_ / 2;	//U
			frame_->linesize[2] = width_ / 2;	//V
		}
		else if (frame_->format == AV_PIX_FMT_RGB24) {
			frame_->linesize[0] = width_ * 3;
		}
		// ����AVFrame�ռ䣬ʹ��Ĭ�϶���
		auto re = av_frame_get_buffer(frame_, 0);
		if (re != 0) {
			char buf[1024] = { 0 };
			av_strerror(re, buf, sizeof(buf) - 1);
			cout << buf << endl;
			av_frame_free(&frame_);
			return NULL;
		}
	}
	if (!frame_) {
		return NULL;
	}
	//��ȡһ֡����
	if (frame_->format == AV_PIX_FMT_YUV420P) {
		ifs_.read((char*)frame_->data[0],
			frame_->linesize[0] * height_);		//Y
		ifs_.read((char*)frame_->data[1],
			frame_->linesize[1] * height_ / 2);		//U
		ifs_.read((char*)frame_->data[2],
			frame_->linesize[2] * height_ / 2);		//V
	}
	else if (frame_->format == AV_PIX_FMT_RGB24) {
		ifs_.read((char *)frame_->data[0], frame_->linesize[0] * height_);
	}
	else {  // RGBA ARGB BGRA 32
		ifs_.read((char*)frame_->data[0], frame_->linesize[0] * height_);
	}
	if (ifs_.gcount() == 0) {
		return NULL;
	}

	return frame_;
}
