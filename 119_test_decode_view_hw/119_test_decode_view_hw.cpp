#include <iostream>
#include <fstream>
#include "xvideo_view.h"
using namespace std;

extern "C" { //指定函数是c语言函数，函数名不包含重载标注
//引用ffmpeg头文件
#include <libavcodec/avcodec.h>
//#include <libavutil/frame.h>
//#include <libswscale/swscale.h>
#include <libavutil/opt.h>
}
//预处理指令导入库
#pragma comment(lib,"avcodec.lib")
#pragma comment(lib,"avutil.lib")
//#pragma comment(lib, "swscale.lib")

int main(int argc, char* argv[])
{
	auto view = XVideoView::Create();
	// 1、分割h264 存入AVPacket
	// ffmpeg -i v1080.mp4 -s 400x300 test.h264
	string filename = "test.h264";
	ifstream ifs(filename, ios::binary);
	if (!ifs) return -1;
	unsigned char inbuf[4096] = { 0 };

	AVCodecID codec_id = AV_CODEC_ID_H264;


	// 1、找解码器
	auto codec = avcodec_find_decoder(codec_id);

	// 2、创建上下文
	auto c = avcodec_alloc_context3(codec);

	// 硬件加速格式 DXVA2
	auto hw_type = AV_HWDEVICE_TYPE_DXVA2;
	///////////////////////////////////////////////////
	///// 打印所有支持的硬件加速方式
	for (int i = 0; ; i++) {
		auto config = avcodec_get_hw_config(codec, i);
		if (!config) {
			break;
		}
		if (config->device_type) {
			cout << av_hwdevice_get_type_name(config->device_type) << endl;
		}
	}

	// 初始化硬件加速上下文
	AVBufferRef *hw_ctx = nullptr;
	av_hwdevice_ctx_create(&hw_ctx, hw_type, NULL, NULL, 0);

	// 设定硬件GPU加速
	//c->hw_device_ctx = av_buffer_ref(hw_ctx);
	c->thread_count = 16;
	// 3、打开上下文
	avcodec_open2(c, NULL, NULL);

	// 分割上下文
	auto parser = av_parser_init(codec_id);
	auto pkt = av_packet_alloc();
	auto frame = av_frame_alloc();
	auto hw_frame = av_frame_alloc(); //硬解码使用
	auto begin = NowMs();
	int count = 0;  // 解码统计
	bool is_init_win = false;
	while (!ifs.eof()) {
		ifs.read((char *)inbuf, sizeof(inbuf));
		int data_size = ifs.gcount();  //读取的字节数
		if (data_size <= 0) break;
		if (ifs.eof()) {
			ifs.clear();
			ifs.seekg(0, ios::beg);
		}
		auto data = inbuf;

		while (data_size > 0) {  //一次有多帧数据
			// 通过0001 截断输出到avpact
			int ret = av_parser_parse2(parser, c,
				&pkt->data, &pkt->size,		//输出
				data, data_size,			//输入
				AV_NOPTS_VALUE, AV_NOPTS_VALUE, 0
			);
			data += ret;
			data_size -= ret;	//已处理
			if (pkt->size) {
				//cout << pkt->size << " " << flush;
				// 发送packet到解码线程
				ret = avcodec_send_packet(c, pkt);
				if (ret < 0) {
					break;
				}
				// 获取多帧解码数据
				while (ret >= 0) {
					// 每次会调用av_frame_unref
					ret = avcodec_receive_frame(c, frame);
					if (ret < 0) {
						break;
					}
					auto pframe = frame;    // 为了同时支持硬解码和软解码
					if (c->hw_device_ctx) {  //硬解码
						// 硬解码转换GPU -》CPU 显存 -》内存
						av_hwframe_transfer_data(hw_frame, frame, 0);
						pframe = hw_frame;
					}
					cout << frame->format << " " << flush;
					///////////////////////////////
					/// 第一帧初始化窗口
					if (!is_init_win) {
						is_init_win = true;
						view->Init(pframe->width, pframe->height, (XVideoView::Format)pframe->format);
					}
					view->DrawFrame(pframe);
					count++;
					auto cur = NowMs();
					if (cur - begin >= 1000) {  // 1秒钟计算一次
						cout << "\nfps = " << count << endl;
						count = 0;
						begin = cur;
					}
				}
			}
		}
		
	}
	/// 取出缓存数据
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