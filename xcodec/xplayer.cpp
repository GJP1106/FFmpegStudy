#include "xplayer.h"
#include "xaudio_play.h"

void XPlayer::Do(AVPacket * pkt)
{
	if (audio_decode_.is_open())
	    audio_decode_.Do(pkt);
	if (video_decode_.is_open())
	    video_decode_.Do(pkt);
}

bool XPlayer::Open(const char * url, void * winid)
{
	// ���װ
	if (!demux_.Open(url)) {
		return false;
	}
	// ��Ƶ����
	auto vp = demux_.CopyVideoPara();
	if (vp) {
		if (!video_decode_.Open(vp->para)) {
			return false;
		}
		video_decode_.set_stream_index(demux_.video_index());
		//����
		video_decode_.set_block_size(100);
		// ��Ƶ��Ⱦ
		if (!view_) {
			view_ = XVideoView::Create();
		}
		view_->set_win_id(winid);
		if (!view_->Init(vp->para)) {
			return false;
		}
	}
	auto ap = demux_.CopyAudioPara();
	if (ap) {
		// ��ʼ����Ƶ����
		if (!audio_decode_.Open(ap->para)) {
			return false;
		}
		// ���ڹ�����Ƶ����
		audio_decode_.set_stream_index(demux_.audio_index());
		//����
		audio_decode_.set_block_size(100);
		// ����Ƶ����
		audio_decode_.set_frame_cache(true);
		// ��ʼ����Ƶ����
		XAudioPlay::Instace()->Open(*ap);
	}
	else {
		demux_.set_syn_type(XSYN_VIDEO);		//������Ƶͬ��
	}
	// ���װ���ݴ�����ǰ��
	demux_.set_next(this);
	return true;
}

void XPlayer::Stop()
{
	XThread::Stop();
	demux_.Stop();
	audio_decode_.Stop();
	video_decode_.Stop();
	Wait();
	demux_.Wait();
	audio_decode_.Wait();
	video_decode_.Wait();
	if (view_) {
		view_->Close();
		delete view_;
		view_ = nullptr;
	}
	XAudioPlay::Instace()->Close();
}

void XPlayer::Main()
{
	long long syn = 0;
	auto au = XAudioPlay::Instace();
	auto ap = demux_.CopyAudioPara();
	auto vp = demux_.CopyVideoPara();
	if (!ap) return;
	while (!is_exit_) {
		syn = XRescale(au->cur_pts(), ap->time_base, vp->time_base);
		audio_decode_.set_syn_pts(au->cur_pts() + 10000);
		video_decode_.set_syn_pts(syn);
		MSleep(1);
	}
}

void XPlayer::Start()
{
	demux_.Start();
	if (video_decode_.is_open())
		video_decode_.Start();
	if (audio_decode_.is_open())
		audio_decode_.Start();
	XThread::Start();
}

void XPlayer::Update()
{
	// ��Ⱦ��Ƶ
	if (view_) {
		auto f = video_decode_.GetFrame();
		if (f) {
			view_->DrawFrame(f);
			XFreeFrame(&f);
		}
	}

	// ��Ƶ����
	auto au = XAudioPlay::Instace();
	auto f = audio_decode_.GetFrame();
	if (!f) return;
	au->Push(f);
	XFreeFrame(&f);
}
