#pragma once
#include "xtools.h"
#include "XDecode.h"
class XCODEC_API XDecodeTask : public XThread
{
public:
	// �򿪽�����
	bool Open(AVCodecParameters *para);
	// ������������
	void Do(AVPacket* pkt) override;
	// �߳�������
	void Main() override;
	// �̰߳�ȫ ���ص�ǰ��Ҫ��Ⱦ��AVFrame�� ���û�з���nullptr
	// need_view_������Ⱦ
	// ���ؽ����Ҫ�� XFreeFrame�ͷ�
	AVFrame* GetFrame();

	void set_stream_index(int i) { stream_index_ = i; }

	void set_frame_cache(bool is) { frame_cache = is; }
	bool is_open() { return is_open_; }
private:
	bool is_open_ = false;
	int stream_index_ = 0;
	std::mutex mux_;
	XDecode decode_;
	XAVPacketList pkt_list_;
	AVFrame *frame_ = nullptr;	//�����洢
	bool need_view_ = false;		//�Ƿ���Ҫ��Ⱦ��ÿֻ֡��Ⱦһ�Σ�ͨ��GetFrame
	std::list<AVFrame*> frames_;	//�洢��Ƶ����
	bool frame_cache = false;		//�Ƿ񻺳�frame����
};

