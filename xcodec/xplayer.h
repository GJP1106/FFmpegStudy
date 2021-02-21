#pragma once
#include "xtools.h"
#include "xdemuxtask.h"
#include "xdecodetask.h"
#include "xvideo_view.h"
class XCODEC_API XPlayer : public XThread
{
public:
	// �ص���������Ƶ��
	void Do(AVPacket* pkt) override;
	// ������Ƶ ��ʼ�����ź���Ⱦ
	bool Open(const char* url, void* winid);
	void Stop();
	// ���߳� ����ͬ��
	void Main() override;
	// ���� ���װ ����Ƶ���� �� ����ͬ�����߳�
	void Start();
	// ��Ⱦ��Ƶ ������Ƶ
	void Update();
protected:
	XDemuxTask demux_;				//���װ
	XDecodeTask audio_decode_;		//��Ƶ����
	XDecodeTask video_decode_;		//��Ƶ����
	XVideoView* view_ = nullptr;	//��Ƶ��Ⱦ
};

