#pragma once
// ��װ�ͽ��װ����
#include <mutex>
#include "xtools.h"
struct AVFormatContext;
struct AVCodecParameters;
struct AVCodecContext;
struct AVPacket;
struct XRational {
	int num;
	int den;
};
class XCODEC_API XFormat
{
public:
	// ���Ʋ����� �̰߳�ȫ
	//
	bool CopyPara(int stream_index, AVCodecParameters* dst);
	bool CopyPara(int stream_index, AVCodecContext* dst);
	// ��������ָ�� ������Ƶ����
	std::shared_ptr<XPara> CopyVideoPara();
	std::shared_ptr<XPara> CopyAudioPara();
	// ���������ģ� ���������ϴε����õ�ֵ���������NULL,  �൱�ڹر�������
	// �̰߳�ȫ
	void set_c(AVFormatContext *c);
	int audio_index() { return audio_index_; }
	int video_index() { return video_index_; }
	XRational video_time_base() { return video_time_base_; }
	XRational audio_time_base() { return audio_time_base_; }

	// ����timebase����ʱ��
	bool RescaleTime(AVPacket *pkt, long long offset_pts, XRational time_base);
	bool RescaleTime(AVPacket *pkt, long long offset_pts, AVRational* time_base);
	// ��pts dts durationֵתΪ����
	long long RescaleToMs(long long pts, int index);
	int video_codec_id() { return video_codec_id_; }
	//�ж��Ƿ�ʱ
	bool IsTimeout() {
		if (NowMs() - last_time_ > time_out_ms_) {    //��ʱ
			last_time_ = NowMs();
			is_connected_ = false;
			return true;
		}
		return false;
	}
	// �趨��ʱʱ��
	void set_time_out_ms(int ms);
	bool is_connected() { return is_connected_; }
protected:
	int time_out_ms_ = 0;				// ��ʱʱ�䣬����
	long long last_time_ = 0;			//�ϴν��յ����ݵ�ʱ��
	bool is_connected_ = false;			// �Ƿ����ӳɹ�
	AVFormatContext* c_ = nullptr;		// ��װ�ͽ��װ������
	std::mutex mux_;					// c_��Դ����
	int video_index_ = 0;				//video��audio��stream�е�����
	int audio_index_ = 1;
	XRational video_time_base_ = {1, 25};
	XRational audio_time_base_ = { 1, 9000 };
	int video_codec_id_ = 0;                   // ������ID
};

