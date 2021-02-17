#include <iostream>
#include <thread>
using namespace std;

extern "C" {
#include <libavformat/avformat.h>
}

#pragma comment(lib, "avformat.lib")
#pragma comment(lib, "avutil.lib")
#pragma comment(lib, "avcodec.lib")

void PrintErr(int err)
{
	char buf[1024] = { 0 };
	av_strerror(err, buf, sizeof(buf) - 1);
	cerr << buf << endl;
	return;
}

#define CERR(err) if (err != 0) { PrintErr(err); getchar(); return -1; }

int main(int argc, char* argv[])
{
	// ��ý���ļ�
	const char *url = "v1080.mp4";
	//////////////////////////////////////////////////////////////////
    //// ���װ
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

	//////////////////////////////////////////////////////////////////
	//// ��װ
	// ������������
	const char *out_url = "out.mp4";
	AVFormatContext *ec = nullptr;
	re = avformat_alloc_output_context2(&ec, NULL, NULL,
		out_url		//�����ļ����Ʋ��װ��ʽ
	);
	CERR(re);
	// �����Ƶ���� ��Ƶ��
	auto mvs = avformat_new_stream(ec, NULL);	//��Ƶ��
	auto mas = avformat_new_stream(ec, NULL);	//��Ƶ��

	// �����IO
	re = avio_open(&ec->pb, out_url, AVIO_FLAG_WRITE);
	CERR(re);

	// ���ñ�������Ƶ������
	// ec->streams[0];
	//mvs->codecpar; // ��Ƶ����(ѹ����ʽ����������)

	if (vs) {
		mvs->time_base = vs->time_base;	//ʱ�������ԭ��Ƶһ��
		// �ӽ��װ���Ʋ���
		avcodec_parameters_copy(mvs->codecpar, vs->codecpar);
	}

	if (as) {
		mas->time_base = as->time_base;
		// �ӽ��װ���Ʋ���
		avcodec_parameters_copy(mas->codecpar, as->codecpar);
	}

	// д���ļ�ͷ
	re = avformat_write_header(ec, NULL);
	CERR(re);

	// ��ӡ���������
	av_dump_format(ec, 0, out_url, 1);
    
	////////////////////////////////////////////////////////
	//// ��ȡ10 ~ 20s֮�����Ƶ��Ƶ ȡ�಻ȡ��
	// �ٶ� 9 11���йؼ�֡ ����ȡ��9��
	double begin_sec = 10.0;  // ��ȡ��ʼʱ��
	double end_sec = 20.0;    // ��ȡ����ʱ��
	long long begin_pts = 0;
	long long end_pts = 0;
	long long begin_audio_pts = 0;  // ��Ƶ�Ŀ�ʼʱ��
	// �����pts ���������ic��pts�� ����Ƶ��Ϊ׼
	if (vs && vs->time_base.num > 0) {
		// sec / timebase = pts
		// pts = sec / (num / den) = sec * (den / num)
		double t = (double)vs->time_base.den / (double)vs->time_base.num;    // ��ĸ / ����
		begin_pts = begin_sec * t;
		end_pts = end_sec * t;
	}
	if (as && as->time_base.num > 0) {
		begin_audio_pts = begin_sec * ((double)as->time_base.den / (double)as->time_base.num);
	}

	// seek����ý�� �ƶ�����10��Ĺؼ�֡
	if (vs) {
		re = av_seek_frame(ic, vs->index, begin_pts,
			AVSEEK_FLAG_FRAME | AVSEEK_FLAG_BACKWARD);    //���ؼ�֡
	}
	CERR(re);

	AVPacket pkt;
	for (;;) {
		re = av_read_frame(ic, &pkt);
		if (re != 0) {
			PrintErr(re);
			break;
		}
		AVStream *in_stream = ic->streams[pkt.stream_index];
		AVStream *out_stream = nullptr;
		long long offset_pts = 0;  //ƫ��pts�����ڽضϵĿ�ͷpts����
		if (pkt.stream_index == vs->index) {
			cout << "��Ƶ��Ϣ:";
			// ������20���˳��� ֻ��10~20s
			if (pkt.pts > end_pts) {
				av_packet_unref(&pkt);
				break;
			}
			out_stream = ec->streams[0];
			offset_pts = begin_pts;
		}
		else if (pkt.stream_index == as->index) {
			cout << "��Ƶ��Ϣ:";
			out_stream = ec->streams[1];
			offset_pts = begin_audio_pts;
		}
		cout << pkt.pts << " : " << pkt.dts << " : " << pkt.size << endl;
		// ���¼���pts dts duration
		// a * bq(����basetime) / cq(���basetime)

		pkt.pts = av_rescale_q_rnd(pkt.pts - offset_pts, in_stream->time_base,
			out_stream->time_base, (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
		pkt.dts = av_rescale_q_rnd(pkt.dts - offset_pts, in_stream->time_base,
			out_stream->time_base, (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
		pkt.duration = av_rescale_q(pkt.duration, in_stream->time_base, out_stream->time_base);
		pkt.pos = -1;
		// д������Ƶ֡ ������pkt
		re = av_interleaved_write_frame(ec,
			&pkt);
		if (re != 0) {
			PrintErr(re);
		}
		//av_packet_unref(&pkt);
		//this_thread::sleep_for(100ms);
	}
	// д���β �����ļ�ƫ������
	re = av_write_trailer(ec);
	if (re != 0) {
		PrintErr(re);
	}

	avformat_close_input(&ic);
	avio_closep(&ec->pb);
	avformat_free_context(ec);
	ec = NULL;
	return 0;
}