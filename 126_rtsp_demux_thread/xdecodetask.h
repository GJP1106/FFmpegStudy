#pragma once
#include "xtools.h"
#include "XDecode.h"
class XDecodeTask : public XThread
{
public:
	// �򿪽�����
	bool Open(AVCodecParameters *para);
	// ������������
	void Do(AVPacket* pkt) override;
	// �߳�������
	void Main() override;
private:
	std::mutex mux_;
	XDecode decode_;
	XAVPacketList pkt_list_;
	AVFrame *frame_ = nullptr;	//�����洢
};

