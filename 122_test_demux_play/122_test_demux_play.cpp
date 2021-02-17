#include <iostream>
#include <thread>
#include "xdecode.h"
#include "xvideo_view.h"
using namespace std;

extern "C" {
#include <libavformat/avformat.h>
}

#pragma comment(lib, "avformat.lib")
#pragma comment(lib, "avutil.lib")
#pragma comment(lib, "avcodec.lib")


/*void PrintErr(int err)
{
	char buf[1024] = { 0 };
	av_strerror(err, buf, sizeof(buf) - 1);
	cerr << buf << endl;
	return;
}*/

#define CERR(err) if (err != 0) { PrintErr(err); getchar(); return -1; }

int main(int argc, char* argv[])
{
	// ��ý���ļ�
	const char *url = "v1080.mp4";
	// ���װ����������
	AVFormatContext *ic = nullptr;
	auto re = avformat_open_input(&ic, url,
		NULL,	 // ��װ����ʽ��NULL �Զ�̽�� ���ݺ�׺�������ļ�ͷ
		NULL     // �������ã� rtsp��Ҫ����
	);
	CERR(re);
	// ��ȡý����Ϣ�� ��ͷ����ʽ
	re = avformat_find_stream_info(ic, NULL);
	CERR(re);

	av_dump_format(ic, 0, url,
		0   // 0��ʾ�����������룬 1 �����
	);
	AVStream *as = nullptr; // ��Ƶ��
	AVStream *vs = nullptr; // ��Ƶ��
	for (int i = 0; i < ic->nb_streams; i++) {
		if (ic->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
			as = ic->streams[i];
			cout << "==================��Ƶ==============" << endl;
			cout << "sample_rate:" << as->codecpar->sample_rate << endl;
		}
		else if (ic->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
			vs = ic->streams[i];
			cout << "==================��Ƶ==============" << endl;
			cout << "width:" << vs->codecpar->width << endl;
			cout << "height:" << vs->codecpar->height << endl;
		}
	}

	// ��Ƶ������ID
	auto video_codec_id = vs->codecpar->codec_id;	// ��Ƶ������ID
	auto video_par = vs->codecpar;					// ��Ƶ�������
	/////////////////////////////////////////////////
	/// ��Ƶ��������ʼ��
	XDecode decode;
	auto decode_c = XCodec::Create(video_codec_id, false);
	// ���װ����Ƶ������������ݸ�����������
	avcodec_parameters_to_context(decode_c, video_par);

	// ���õ��������У��̰߳�ȫ���������decode_c �����ⲿʹ��
	decode.set_c(decode_c);
	if (!decode.Open()) {
		cout << "decode Open failed!" << endl;
		return -1;
	}

	// ������������ռ�
	auto frame = decode.CreateFrame();

	//////////////////////////////////////////////
	//// ��Ⱦ��ʼ��
	auto view = XVideoView::Create();
	view->Init(video_par->width, video_par->height, (XVideoView::Format)video_par->format);


	AVPacket pkt;
	for (;;) {
		re = av_read_frame(ic, &pkt);
		CERR(re);
		if (pkt.stream_index == vs->index) {
			cout << "��Ƶ��Ϣ:";
			// ������Ƶ
			if (decode.Send(&pkt)) {
				while (decode.Recv(frame))
				{
					cout << frame->pts << " " << endl;
					view->DrawFrame(frame);  //��Ⱦ��Ƶ
				}
			}
		}
		else if (pkt.stream_index == as->index) {
			//cout << "��Ƶ��Ϣ:";
		}
		//cout << pkt.pts << " : " << pkt.dts << " : " << pkt.size << endl;
		av_packet_unref(&pkt);
		//this_thread::sleep_for(100ms);
	}

	av_frame_free(&frame);
	avformat_close_input(&ic);
	return 0;
}