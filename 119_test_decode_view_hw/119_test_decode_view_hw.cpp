#include <iostream>
#include <fstream>
#include "xvideo_view.h"
using namespace std;

extern "C" { //ָ��������c���Ժ��������������������ر�ע
//����ffmpegͷ�ļ�
#include <libavcodec/avcodec.h>
//#include <libavutil/frame.h>
//#include <libswscale/swscale.h>
#include <libavutil/opt.h>
}
//Ԥ����ָ����
#pragma comment(lib,"avcodec.lib")
#pragma comment(lib,"avutil.lib")
//#pragma comment(lib, "swscale.lib")

int main(int argc, char* argv[])
{
	auto view = XVideoView::Create();
	// 1���ָ�h264 ����AVPacket
	// ffmpeg -i v1080.mp4 -s 400x300 test.h264
	string filename = "test.h264";
	ifstream ifs(filename, ios::binary);
	if (!ifs) return -1;
	unsigned char inbuf[4096] = { 0 };

	AVCodecID codec_id = AV_CODEC_ID_H264;


	// 1���ҽ�����
	auto codec = avcodec_find_decoder(codec_id);

	// 2������������
	auto c = avcodec_alloc_context3(codec);

	// Ӳ�����ٸ�ʽ DXVA2
	auto hw_type = AV_HWDEVICE_TYPE_DXVA2;
	///////////////////////////////////////////////////
	///// ��ӡ����֧�ֵ�Ӳ�����ٷ�ʽ
	for (int i = 0; ; i++) {
		auto config = avcodec_get_hw_config(codec, i);
		if (!config) {
			break;
		}
		if (config->device_type) {
			cout << av_hwdevice_get_type_name(config->device_type) << endl;
		}
	}

	// ��ʼ��Ӳ������������
	AVBufferRef *hw_ctx = nullptr;
	av_hwdevice_ctx_create(&hw_ctx, hw_type, NULL, NULL, 0);

	// �趨Ӳ��GPU����
	//c->hw_device_ctx = av_buffer_ref(hw_ctx);
	c->thread_count = 16;
	// 3����������
	avcodec_open2(c, NULL, NULL);

	// �ָ�������
	auto parser = av_parser_init(codec_id);
	auto pkt = av_packet_alloc();
	auto frame = av_frame_alloc();
	auto hw_frame = av_frame_alloc(); //Ӳ����ʹ��
	auto begin = NowMs();
	int count = 0;  // ����ͳ��
	bool is_init_win = false;
	while (!ifs.eof()) {
		ifs.read((char *)inbuf, sizeof(inbuf));
		int data_size = ifs.gcount();  //��ȡ���ֽ���
		if (data_size <= 0) break;
		if (ifs.eof()) {
			ifs.clear();
			ifs.seekg(0, ios::beg);
		}
		auto data = inbuf;

		while (data_size > 0) {  //һ���ж�֡����
			// ͨ��0001 �ض������avpact
			int ret = av_parser_parse2(parser, c,
				&pkt->data, &pkt->size,		//���
				data, data_size,			//����
				AV_NOPTS_VALUE, AV_NOPTS_VALUE, 0
			);
			data += ret;
			data_size -= ret;	//�Ѵ���
			if (pkt->size) {
				//cout << pkt->size << " " << flush;
				// ����packet�������߳�
				ret = avcodec_send_packet(c, pkt);
				if (ret < 0) {
					break;
				}
				// ��ȡ��֡��������
				while (ret >= 0) {
					// ÿ�λ����av_frame_unref
					ret = avcodec_receive_frame(c, frame);
					if (ret < 0) {
						break;
					}
					auto pframe = frame;    // Ϊ��ͬʱ֧��Ӳ����������
					if (c->hw_device_ctx) {  //Ӳ����
						// Ӳ����ת��GPU -��CPU �Դ� -���ڴ�
						av_hwframe_transfer_data(hw_frame, frame, 0);
						pframe = hw_frame;
					}
					cout << frame->format << " " << flush;
					///////////////////////////////
					/// ��һ֡��ʼ������
					if (!is_init_win) {
						is_init_win = true;
						view->Init(pframe->width, pframe->height, (XVideoView::Format)pframe->format);
					}
					view->DrawFrame(pframe);
					count++;
					auto cur = NowMs();
					if (cur - begin >= 1000) {  // 1���Ӽ���һ��
						cout << "\nfps = " << count << endl;
						count = 0;
						begin = cur;
					}
				}
			}
		}
		
	}
	/// ȡ����������
	int ret = avcodec_send_packet(c, NULL);
	while (ret >= 0) {
		ret = avcodec_receive_frame(c, frame);
		if (ret < 0) {
			break;
		}
		view->DrawFrame(frame);
		cout << frame->format << "---" << flush;
	}
	av_parser_close(parser);
	avcodec_free_context(&c);
	av_frame_free(&frame);
	av_packet_free(&pkt);

	return 0;
}