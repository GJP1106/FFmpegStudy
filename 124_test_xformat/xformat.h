#pragma once
// ��װ�ͽ��װ����
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
	// ���Ʋ����� �̰߳�ȫ
	//
	bool CopyPara(int stream_index, AVCodecParameters* dst);
	bool CopyPara(int stream_index, AVCodecContext* dst);
	// ���������ģ� ���������ϴε����õ�ֵ���������NULL,  �൱�ڹر�������
	// �̰߳�ȫ
	void set_c(AVFormatContext *c);
	int audio_index() { return audio_index_; }
	int video_index() { return video_index_; }
	XRational video_time_base() { return video_time_base_; }
	XRational audio_time_base() { return audio_time_base_; }

	// ����timebase����ʱ��
	bool RescaleTime(AVPacket *pkt, long long offset_pts, XRational time_base);
	int video_codec_id() { return video_codec_id_; }
protected:
	AVFormatContext* c_ = nullptr;		// ��װ�ͽ��װ������
	std::mutex mux_;					// c_��Դ����
	int video_index_ = 0;				//video��audio��stream�е�����
	int audio_index_ = 1;
	XRational video_time_base_ = {1, 25};
	XRational audio_time_base_ = { 1, 9000 };
	int video_codec_id_ = 0;                   // ������ID
};

