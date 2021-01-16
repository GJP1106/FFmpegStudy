#include <iostream>
#include <fstream>
#include "xvideo_view.h"
#include "xencode.h"
using namespace std;

extern "C" { //ָ��������c���Ժ��������������������ر�ע
//����ffmpegͷ�ļ�
#include <libavcodec/avcodec.h>
//#include <libavutil/frame.h>
#include <libswscale/swscale.h>
#include <libavutil/opt.h>
}
//Ԥ����ָ����
#pragma comment(lib,"avcodec.lib")
#pragma comment(lib,"avutil.lib")
#pragma comment(lib, "swscale.lib")

int encWidth = 800;
int encHeight = 600;

int main(int argc, char* argv[])
{
	auto view = XVideoView::Create();
	// 1���ָ�h264 ����AVPacket
	// ffmpeg -i v1080.mp4 -s 400x300 test.h264
	string filename = "test_1080p.h264";
	string encodefile = "400_300_after_decode.h264";
	ifstream ifs(filename, ios::binary);
	if (!ifs) return -1;
	ofstream enofs;
	enofs.open(encodefile, ios::binary);
	//if (!enofs) return -1;
	unsigned char inbuf[4096] = { 0 };

	AVCodecID codec_id = AV_CODEC_ID_H264;

	XEncode en;
	// 1���ҽ�����
	auto codec = avcodec_find_decoder(codec_id);
	auto enctx = en.Create(codec_id);
	if (!enctx) {
		cerr << "AVCodecContext not find!" << endl;
		return -1;
	}
	// 2������������
	auto c = avcodec_alloc_context3(codec);
	c->thread_count = 16;
	// 3����������
	avcodec_open2(c, NULL, NULL);
	enctx->width = encWidth;
	enctx->height = encHeight;
	en.set_c(enctx);
	en.SetOpt("crf", 18);
	en.SetOpt("preset", "ultrafast");
	en.Open();
	auto enFrame = en.CreateFrame();

	// �ָ�������
	auto parser = av_parser_init(codec_id);
	auto pkt = av_packet_alloc();
	auto frame = av_frame_alloc();
	auto begin = NowMs();
	int count = 0;  // ����ͳ��
	bool is_init_win = false;
	SwsContext *transYuv = nullptr;
	while (!ifs.eof()) {
		ifs.read((char *)inbuf, sizeof(inbuf));
		int data_size = ifs.gcount();  //��ȡ���ֽ���
		if (data_size <= 0) break;
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
				cout << pkt->size << " " << flush;
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
					//cout << frame->format << " " << flush;
					///////////////////////////////
					/// ��һ֡��ʼ������
					if (!is_init_win) {
						is_init_win = true;
						view->Init(frame->width, frame->height, (XVideoView::Format)frame->format);
					}

					view->DrawFrame(frame);
					transYuv = sws_getCachedContext(transYuv,
						frame->width, frame->height,
						AV_PIX_FMT_YUV420P,
						encWidth, encHeight,
						AV_PIX_FMT_YUV420P,
						SWS_BILINEAR, 0, 0, 0);
					if (!transYuv) {
						cerr << "sws_getCachedContext failed!" << endl;
					}

					int re = sws_scale(transYuv, frame->data,
						frame->linesize, 0,
						frame->height, enFrame->data, enFrame->linesize);
					enFrame->pts = count;
					auto enpkt = en.Encode(enFrame);
					if (enpkt) {
						count++;
						enofs.write((char *)enpkt->data, enpkt->size);
						av_packet_free(&enpkt);
					}
					auto cur = NowMs();
					if (cur - begin >= 100) {  // 1/10���Ӽ���һ��
						cout << "\nfps = " << count * 10 << endl;
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
		cout << frame->format << "---" << flush;
	}
	av_parser_close(parser);
	avcodec_free_context(&c);
	av_frame_free(&frame);
	av_packet_free(&pkt);

	return 0;
}