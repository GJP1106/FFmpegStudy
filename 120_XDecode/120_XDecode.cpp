#include <iostream>
#include <fstream>
#include "xvideo_view.h"
#include "xcodec.h"
#include "xdecode.h"
#include <windows.h>
#include <d3d9.h>
using namespace std;

/* 作业：1、将h264视频文件解码后改变像素尺寸后重新编码存入文件
         2、像素尺寸由800x600改为400x300
		 3、基于117_test_decode
		 4、重新编码后用vlc播放测试

*/
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

struct DXVA2DevicePriv {
	HMODULE d3dlib;
	HMODULE dxva2lib;
	HANDLE device_handle;
	IDirect3D9* d3d9;
	IDirect3DDevice9* d3d9device;
};

void DrawFrame(AVFrame* frame, AVCodecContext *c)
{
	if (!frame->data[3] || !c) return;
	cout << "D" << flush;
	auto surface = (IDirect3DSurface9*)frame->data[3];
	auto ctx = (AVHWDeviceContext*)c->hw_device_ctx->data;
	auto priv = (DXVA2DevicePriv*)ctx->user_opaque;
	auto device = priv->d3d9device;
	static HWND hwnd = nullptr;
	static RECT viewport;
	if (!hwnd) {
		hwnd = CreateWindow(L"DX", L"Test DXVA", WS_OVERLAPPEDWINDOW,
			200, 200, frame->width, frame->height, 0, 0, 0, 0);
		ShowWindow(hwnd, 1);
		UpdateWindow(hwnd);
		viewport.left = 0;
		viewport.right = frame->width;
		viewport.top = 0;
		viewport.bottom = frame->height;
	}
	//设置显示窗口句柄
	device->Present(&viewport, &viewport, hwnd, 0);
	//后台缓冲表面
	static IDirect3DSurface9* back = nullptr;
	if (!back)
		device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &back);
	device->StretchRect(surface, 0, back, &viewport, D3DTEXF_LINEAR);
}

int main(int argc, char* argv[])
{
	WNDCLASSEX wc;
	memset(&wc, 0, sizeof(wc));
	wc.cbSize = sizeof(wc);
	wc.lpfnWndProc = DefWindowProc;   //消息函数
	wc.lpszClassName = L"DX";
	RegisterClassEx(&wc);
	//auto view = XVideoView::Create();
	// 1、分割h264 存入AVPacket
	// ffmpeg -i v1080.mp4 -s 400x300 test.h264
	string filename = "test.h264";
	ifstream ifs(filename, ios::binary);
	if (!ifs) return -1;
	unsigned char inbuf[4096] = { 0 };

	AVCodecID codec_id = AV_CODEC_ID_H264;
	XDecode de;
	auto c = de.Create(codec_id, false);
	de.set_c(c);
	de.InitHW();   //硬件加速
	de.Open();

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
				if (!de.Send(pkt)) {
					break;
				}

			    // 获取多帧解码数据
				while (de.Recv(frame, false)) {

					cout << frame->format << " " << flush;
					///////////////////////////////
					/// 第一帧初始化窗口
					if (!is_init_win) {
						is_init_win = true;
						//view->Init(frame->width, frame->height, (XVideoView::Format)frame->format);
					}
					DrawFrame(frame, c);
					//view->DrawFrame(frame);
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
	auto frames = de.End();
	for (auto f : frames) {
		//view->DrawFrame(f);
		av_frame_free(&f);
	}
	av_parser_close(parser);
	avcodec_free_context(&c);
	av_frame_free(&frame);
	av_packet_free(&pkt);


	return 0;
}