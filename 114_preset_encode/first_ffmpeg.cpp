#include <iostream>
#include <fstream>
using namespace std;
extern "C"{ //ָ��������c���Ժ��������������������ر�ע
//����ffmpegͷ�ļ�
#include <libavcodec/avcodec.h>
#include <libavutil/frame.h>
#include <libswscale/swscale.h>
#include <libavutil/opt.h>
}
//Ԥ����ָ����
#pragma comment(lib,"avcodec.lib")
#pragma comment(lib,"avutil.lib")
#pragma comment(lib, "swscale.lib")

#define YUV_FILE "400_300_25.yuv"
#define RGBA_FILE "800_600_25.rgba"
#define YUV_FILE_1 "400_300_25_1.yuv"

void Encode(int index, char *str)
{
	string filename = "400_300_25_preset";
	AVCodecID codec_id = AV_CODEC_ID_H264;
	if (index > 1) {
		string codec = str;
		if (codec == "h265" || codec == "hevc") {
			codec_id = AV_CODEC_ID_HEVC;
		}
	}
	if (codec_id == AV_CODEC_ID_H264) {
		filename += ".h264";
	}
	else if (codec_id == AV_CODEC_ID_HEVC) {
		filename += ".h265";
	}
	ofstream ofs;
	ofs.open(filename, ios::binary);
	/// 1���ҵ������� AV_CODEC_ID_HEVC(h265)
	auto codec = avcodec_find_encoder(codec_id);
	if (!codec) {
		cerr << "codec not find!" << endl;
		return;
	}
	/// 2������������
	auto c = avcodec_alloc_context3(codec);
	if (!c) {
		cerr << "avcodec_alloc_context3 failed!" << endl;
		return;
	}

	/// 3���趨�����Ĳ���
	c->width = 400;
	c->height = 300;
	// ֡ʱ�����ʱ�䵥λ  pts * time_base = ����ʱ��(��)
	c->time_base = { 1, 25 };			// ����	1 / 25

	c->pix_fmt = AV_PIX_FMT_YUV420P;	// Ԫ�������ظ�ʽ��������㷨���
	c->thread_count = 16;				// �����߳���������ͨ������ϵͳ�ӿڻ�ȡcpu��������

	// Ԥ�����������
	//c->max_b_frames = 0;  //B֡��Ϊ0��������ʱ������ռ�
	//int optRet = av_opt_set(c->priv_data, "preset", "ultrafast", 0);  //����ٶ�
	//if (optRet != 0) {
	//	cout << "preset failed!" << endl;
	//}
	//optRet = av_opt_set(c->priv_data, "tune", "zerolatency", 0);  //����ʱ�� h265��֧��B֡
	//if (optRet != 0) {
	//	cout << "preset failed!" << endl;
	//}

	////////////////////////////////////////////////////////
	//// ABR ƽ��������
	int br = 400000; // 400kb
	// c->bit_rate = br;

	//////////////////////////////////////////////////////////
	//// CQP�㶨����  H264�е�QP��Χ��0��51
	// x264Ĭ��23 ��Ч���Ϻ� 18
	// x265Ĭ��28 �� Ч���Ϻ� 25
	//av_opt_set_int(c->priv_data, "qp", 18, 0);


	//////////////////////////////////////
	// �㶨������(CBR) ����MP4��֧��NAL��䣬�������ļ�����Ϊ(MPEG-2 TS)
	/*c->rc_min_rate = br;
	c->rc_max_rate = br;
	c->rc_buffer_size = br;
	c->bit_rate = br;
	av_opt_set(c->priv_data, "nal-hrd", "cbr", 0);*/

	////////////////////////////////////////////////////
	// �㶨�������� (CRF)
	av_opt_set_int(c->priv_data, "crf", 23, 0);
	////////////////////////////////////////////////////
    // Լ������(VBV) Constrained Encoding
	av_opt_set_int(c->priv_data, "crf", 23, 0);
	c->rc_max_rate = br;
	c->rc_buffer_size = br * 2;
	/// 4.�򿪱�����������
	int re = avcodec_open2(c, codec, NULL);
	if (re != 0) {
		char buf[1024] = { 0 };
		av_strerror(re, buf, sizeof(buf) - 1);
		cerr << "avcodec_open2 faild!" << buf << endl;
		return;
	}
	cout << "avcodec_open2 success!" << endl;
	/// ������AVFrame�ռ� δѹ������
	auto frame = av_frame_alloc();
	frame->width = c->width;
	frame->height = c->height;
	frame->format = c->pix_fmt;
	re = av_frame_get_buffer(frame, 0);
	if (re != 0) {
		char buf[1024] = { 0 };
		av_strerror(re, buf, sizeof(buf) - 1);
		cerr << "av_frame_get_buffer faild!" << buf << endl;
		return;
	}
	auto packet = av_packet_alloc();
	// ʮ����Ƶ��250֡
	for (int i = 0; i < 250; i++) {
		// ����AVFrame ����  ÿ֡���ݲ�ͬ
		// Y
		for (int y = 0; y < c->height; y++) {
			for (int x = 0; x < c->width; x++) {
				frame->data[0][y * frame->linesize[0] + x] = x + y + i * 3;
			}
		}
		//UV
		for (int y = 0; y < c->height / 2; y++) {
			for (int x = 0; x < c->width / 2; x++) {
				frame->data[1][y * frame->linesize[1] + x] = 128 + y + i * 2;
				frame->data[2][y * frame->linesize[2] + x] = 64 + x + i * 5;
			}
		}
		frame->pts = i;   // ��ʾʱ��
		// ����δѹ��֡���߳���ѹ��
		re = avcodec_send_frame(c, frame);
		if (re != 0) {

			break;
		}
		while (re >= 0) { //���ض�֡
			// ����ѹ��֡��һ��ǰ���ε��÷��ؿ�(���壬���̷��أ�����δ���)
            // �������ڶ������߳���
			re = avcodec_receive_packet(c, packet);
			if (re == AVERROR(EAGAIN) || re == AVERROR_EOF) {
				break;
			}
			if (re < 0) {
				char buf[1024] = { 0 };
				av_strerror(re, buf, sizeof(buf) - 1);
				cerr << "avcodec_receive_packet faild!" << buf << endl;
				break;
			}
			cout << packet->size << " " << flush;
			ofs.write((char*)packet->data, packet->size);
			av_packet_unref(packet);
		}
	}
	ofs.close();
	av_packet_free(&packet);
	av_frame_free(&frame);
	/// �ͷű�����������
	avcodec_free_context(&c);
}

void YUVToRGBA()
{
	int width = 400;
	int height = 300;
	int rgb_width = 800;
	int rgb_height = 600;
	// YUV420P ƽ��洢 yyyy yyyy uu vv
	unsigned char *yuv[3] = { 0 };
    int yuv_linesize[3] = { width, width / 2, width / 2 };
	yuv[0] = new unsigned char[width * height];
	yuv[1] = new unsigned char[width * height / 4];
	yuv[2] = new unsigned char[width * height / 4];

	// RGBA����洢 RGBA RGBA
	unsigned char *rgba = new unsigned char[rgb_width * rgb_height * 4];
	int rgba_linesize = rgb_width * 4;

	ifstream ifs;
	ifs.open(YUV_FILE, ios::binary);
	if (!ifs) {
		cerr << "open " << YUV_FILE << " failed!" << endl;
		return;
	}
	ofstream ofs;
	ofs.open(RGBA_FILE, ios::binary);
	if (!ofs) {
		cerr << "open " << RGBA_FILE << " failed!" << endl;
		return;
	}
	SwsContext *yuv2rgb = nullptr;

	for (;;) {
		// ��ȡYUV����
		ifs.read((char *)yuv[0], width * height);
		ifs.read((char *)yuv[1], width * height / 4);
		ifs.read((char *)yuv[2], width * height / 4);
		if (ifs.gcount() == 0) break;

		// YUVתRGB
		yuv2rgb = sws_getCachedContext(
			yuv2rgb,						// ת�������ģ�NULL�´�������NULL�ж������в����Ƿ�һ�£�һֱֱ�ӷ��أ�
											// ��һ��������ǰ�������ٴ���
			width, height,					// ������
			AV_PIX_FMT_YUV420P,				//�������ظ�ʽ
			rgb_width, rgb_height,			//����Ŀ��
			AV_PIX_FMT_RGBA,				//��������ظ�ʽ
			SWS_BILINEAR,					//ѡ��֧�ֱ仯���㷨��˫���Բ�ֵ
			0, 0, 0							//����������(��ʹ��)
		);
		if (!yuv2rgb) {
			cerr << "sws_getCachedContext failed!" << endl;
			return;
		}

		unsigned char* data[1];
		data[0] = rgba;
		int lines[1] = { rgba_linesize };
		int re = sws_scale(yuv2rgb,
			yuv,			//��������
			yuv_linesize,	//����������ֽ��� 
			0,
			height,			//����߶�
			data,			//�������
			lines
		);
		cout << re << " " << flush;
		ofs.write((char *)rgba, rgb_width * rgb_height * 4);
	}

	delete yuv[0];
	delete yuv[1];
	delete yuv[2];
	delete rgba;
	ofs.close();
	ifs.close();
}

void RGBA2YUV()
{
	int width = 400;
	int height = 300;
	int rgb_width = 800;
	int rgb_height = 600;
	// YUV420P ƽ��洢 yyyy yyyy uu vv
	unsigned char *yuv[3] = { 0 };
	int yuv_linesize[3] = { width, width / 2, width / 2 };
	yuv[0] = new unsigned char[width * height];
	yuv[1] = new unsigned char[width * height / 4];
	yuv[2] = new unsigned char[width * height / 4];

	// RGBA����洢 RGBA RGBA
	unsigned char *rgba = new unsigned char[rgb_width * rgb_height * 4];
	int rgba_linesize = rgb_width * 4;

	ifstream ifs;
	ifs.open(RGBA_FILE, ios::binary);
	if (!ifs) {
		cerr << "open " << RGBA_FILE << " failed!" << endl;
		return;
	}
	ofstream ofs;
	ofs.open(YUV_FILE_1, ios::binary);
	if (!ofs) {
		cerr << "open " << YUV_FILE_1 << " failed!" << endl;
		return;
	}
	SwsContext *rgba2yuv = nullptr;

	for (;;) {
		// ��ȡRGBA����
		ifs.read((char*)rgba, rgb_width * rgb_height * 4);
		if (ifs.gcount() == 0) break;

		// YUVתRGB
		rgba2yuv = sws_getCachedContext(
			rgba2yuv,						// ת�������ģ�NULL�´�������NULL�ж������в����Ƿ�һ�£�һֱֱ�ӷ��أ�
											// ��һ��������ǰ�������ٴ���
			rgb_width, rgb_height,			// ������
			AV_PIX_FMT_RGBA,				//�������ظ�ʽ
			width, height,					//����Ŀ��
			AV_PIX_FMT_YUV420P,				//��������ظ�ʽ
			SWS_BILINEAR,					//ѡ��֧�ֱ仯���㷨��˫���Բ�ֵ
			0, 0, 0							//����������(��ʹ��)
		);
		if (!rgba2yuv) {
			cerr << "sws_getCachedContext failed!" << endl;
			return;
		}

		unsigned char* data[1];
		data[0] = rgba;
		int lines[1] = { rgba_linesize };
		int re = sws_scale(rgba2yuv,
			data,			//��������
			lines,	//����������ֽ��� 
			0,
			rgb_height,			//����߶�
			yuv,			//�������
			yuv_linesize
		);
		cout << re << " " << flush;
		ofs.write((char *)yuv[0], width * height);
		ofs.write((char *)yuv[1], width * height / 4);
		ofs.write((char *)yuv[2], width * height / 4);
	}

	delete yuv[0];
	delete yuv[1];
	delete yuv[2];
	delete rgba;
	ofs.close();
	ifs.close();
}

int main(int argc, char* argv[])
{
    cout << "first ffmpeg" << endl;
    cout << avcodec_configuration() << endl;
	// ����frame����
	auto frame1 = av_frame_alloc();

	// ͼ�����
	frame1->width = 400;
	frame1->height = 300;
	frame1->format = AV_PIX_FMT_ARGB;
	// ����ռ�,QT����16���뷽ʽ��ffmpegĬ����32�ֽڶ��룬��С4�ֽڶ���
	int re = av_frame_get_buffer(frame1, 16);
	if (re != 0) {
		char buf[1024] = { 0 };
		av_strerror(re, buf, sizeof(buf));
		cout << buf << endl;
	}
	cout << "frame1 linsize" << frame1->linesize[0] << endl;
	cout << "frame1 linsize" << frame1->linesize[1] << endl;
	cout << "frame1 linsize" << frame1->linesize[2] << endl;
	if (frame1->buf[0]) {
		cout << "frame1 ref count = " <<
			av_buffer_get_ref_count(frame1->buf[0]); // �̰߳�ȫ
		cout << endl;
	}

	auto frame2 = av_frame_alloc();
	av_frame_ref(frame2, frame1);
	cout << "frame1 ref count = " <<
		av_buffer_get_ref_count(frame1->buf[0]); // �̰߳�ȫ
	cout << endl;

	cout << "frame2 ref count = " <<
		av_buffer_get_ref_count(frame2->buf[0]); // �̰߳�ȫ
	cout << endl;

	//���ü���-1������buff����
	av_frame_unref(frame2);
	cout << "av_frame_unref(frame2)" << endl;
	cout << "frame1 ref count = " <<
		av_buffer_get_ref_count(frame1->buf[0]); // �̰߳�ȫ
	cout << endl;

	//���ü���Ϊ1��ֱ��ɾ��buf�ռ䣬���ü�����Ϊ0
	av_frame_unref(frame1);

	// �Ƿ�frame����ռ䣬buf�����ü�����һ
	// buf�Ѿ�Ϊ�գ�ֻɾ��frame����ռ�
	av_frame_free(&frame1);
	av_frame_free(&frame2);
	cout << "lll_test_swa_scale" << endl;
	// ffmpeg -i test.mp4 -s 400x300 400_300_25.yuv
	// 400x300 YUV ת RGBA 800x600 ���浽�ļ�
	//YUVToRGBA();
	cout << "RGBA2YUV" << endl;
	//RGBA2YUV();
	Encode(argc, argv[1]);
    return 0;
}