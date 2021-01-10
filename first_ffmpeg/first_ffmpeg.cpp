#include <iostream>
#include <fstream>
using namespace std;
extern "C"{ //ָ��������c���Ժ��������������������ر�ע
//����ffmpegͷ�ļ�
#include <libavcodec/avcodec.h>
#include <libavutil/frame.h>
#include <libswscale/swscale.h>
}
//Ԥ����ָ����
#pragma comment(lib,"avcodec.lib")
#pragma comment(lib,"avutil.lib")
#pragma comment(lib, "swscale.lib")

#define YUV_FILE "400_300_25.yuv"
#define RGBA_FILE "800_600_25.rgba"
#define YUV_FILE_1 "400_300_25_1.yuv"

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
	RGBA2YUV();
    return 0;
}