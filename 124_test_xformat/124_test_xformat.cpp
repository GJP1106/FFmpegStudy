#include <iostream>
#include <thread>
#include <string>
#include "xdemux.h"
#include "xmux.h"
using namespace std;

extern "C" {
#include <libavformat/avformat.h>
}

#pragma comment(lib, "avformat.lib")
#pragma comment(lib, "avutil.lib")
#pragma comment(lib, "avcodec.lib")

static void PrintErr(int err)
{
	char buf[1024] = { 0 };
	av_strerror(err, buf, sizeof(buf) - 1);
	cerr << buf << endl;
	return;
}

#define CERR(err) if (err != 0) { PrintErr(err); getchar(); return -1; }

int main(int argc, char* argv[])
{
	//////////////////////////////////////////////////
	// �����������
	// ʹ��˵��
	string useage = "124_test_xformat �����ļ� ����ļ� ��ʼʱ��(��) ����ʱ��(��)\n";
	useage += "124_test_xformat v1080.mp4 test_out.mp4 10 20";
	cout << useage << endl;

	if (argc < 3) {
		return -1;
	}
	string input_file = argv[1];
	string out_file = argv[2];
	//// ��ȡ10 ~ 20s֮�����Ƶ��Ƶ ȡ�಻ȡ��
    // �ٶ� 9 11���йؼ�֡ ����ȡ��9��
	int begin_sec = 0;  // ��ȡ��ʼʱ��
	int end_sec = 0;    // ��ȡ����ʱ��
	if (argc > 3) {
		begin_sec = atoi(argv[3]);
	}
	if (argc > 4) {
		end_sec = atoi(argv[4]);
	}
	// ��ý���ļ�
	//const char *url = "v1080.mp4";
	//////////////////////////////////////////////////////////////////
	//// ���װ
	// ���װ����������

	XDemux demux;
	auto demux_c = demux.Open(input_file.c_str());

	demux.set_c(demux_c);

	//////////////////////////////////////////////////////////////////
	//// ��װ
	// ������������
	//const char *out_url = "out.mp4";
	XMux mux;
	auto mux_c = mux.Open(out_file.c_str());
	mux.set_c(mux_c);
	auto mvs = mux_c->streams[mux.video_index()];  //��Ƶ����Ϣ
	auto mas = mux_c->streams[mux.audio_index()];  //��Ƶ����Ϣ
	// ����Ƶ
	if (demux.video_index() >= 0) {
		mvs->time_base.num = demux.video_time_base().num;
		mvs->time_base.den = demux.video_time_base().den;
		// ������Ƶ����
		demux.CopyPara(demux.video_index(), mvs->codecpar);
	}
	// ����Ƶ
	if (demux.audio_index() >= 0) {
		mas->time_base.num = demux.audio_time_base().num;
		mas->time_base.den = demux.audio_time_base().den;
		// ������Ƶ����
		demux.CopyPara(demux.audio_index(), mas->codecpar);
	}

	// д���ļ�ͷ
	mux.WriteHead();

	long long video_begin_pts = 0;
	long long audio_begin_pts = 0;
	long long video_end_pts = 0;

	// ��ʼ�ض����� ���������Ƶ��pts
	if (begin_sec > 0) {
		// ������Ƶ��ʼ���ŵ�pts�ͽ�����pts
		if (demux.video_index() >= 0 && demux.video_time_base().num > 0) {
			double t = (double)demux.video_time_base().den / (double)demux.video_time_base().num;
			video_begin_pts = t * begin_sec;
			video_end_pts = t * end_sec;
			demux.Seek(video_begin_pts, demux.video_index()); //�ƶ�����ʼ֡
		}
		// ��Ƶ��ʼ���ŵ�pts
		if (demux.audio_index() >= 0 && demux.audio_time_base().num > 0) {
			double t = (double)demux.audio_time_base().den / (double)demux.audio_time_base().num;
			audio_begin_pts = t * begin_sec;
		}
	}

	int audio_count = 0;
	int video_count = 0;
	double total_sec = 0;
	AVPacket pkt;
	for (;;) {
		if (!demux.Read(&pkt)) {
			break;
		}
		// ��Ƶ ʱ����ڽ���ʱ��
		if (video_end_pts > 0
			&& pkt.stream_index == demux.video_index()
			&& pkt.pts > video_end_pts) {
			av_packet_unref(&pkt);
			break;
		}
		//pkt.pos = -1;
		if (pkt.stream_index == demux.video_index()) {
			mux.RescaleTime(&pkt, video_begin_pts, demux.video_time_base());
			if (demux.video_time_base().den > 0)
			    total_sec += pkt.duration * ((double)demux.video_time_base().num / (double)demux.video_time_base().den);

			video_count++;
		}
		else if (pkt.stream_index == demux.audio_index()) {
			mux.RescaleTime(&pkt, audio_begin_pts, demux.audio_time_base());
			audio_count++;
		}
		// д������Ƶ֡ ������pkt
		mux.Write(&pkt);
	}
	// д���β �����ļ�ƫ������
	mux.WriteEnd();


	//avformat_close_input(&ic);
	demux.set_c(NULL);
	mux.set_c(NULL);
	cout << "����ļ�" << out_file << ":" << endl;
	cout << "��Ƶ֡:" << video_count << endl;
	cout << "��Ƶ֡:" << audio_count << endl;
	cout << "��ʱ��" << total_sec << endl;
	//avio_closep(&ec->pb);
	//avformat_free_context(ec);
	//ec = NULL;
	return 0;
}