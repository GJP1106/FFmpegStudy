#include <iostream>
#include <fstream>
using namespace std;
extern "C"{ //指定函数是c语言函数，函数名不包含重载标注
//引用ffmpeg头文件
#include <libavcodec/avcodec.h>
#include <libavutil/frame.h>
#include <libswscale/swscale.h>
}
//预处理指令导入库
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
	// YUV420P 平面存储 yyyy yyyy uu vv
	unsigned char *yuv[3] = { 0 };
    int yuv_linesize[3] = { width, width / 2, width / 2 };
	yuv[0] = new unsigned char[width * height];
	yuv[1] = new unsigned char[width * height / 4];
	yuv[2] = new unsigned char[width * height / 4];

	// RGBA交叉存储 RGBA RGBA
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
		// 读取YUV数据
		ifs.read((char *)yuv[0], width * height);
		ifs.read((char *)yuv[1], width * height / 4);
		ifs.read((char *)yuv[2], width * height / 4);
		if (ifs.gcount() == 0) break;

		// YUV转RGB
		yuv2rgb = sws_getCachedContext(
			yuv2rgb,						// 转换上下文，NULL新创建，非NULL判断与现有参数是否一致，一直直接返回，
											// 不一致先清理当前的数据再创建
			width, height,					// 输入宽高
			AV_PIX_FMT_YUV420P,				//输入像素格式
			rgb_width, rgb_height,			//输出的宽高
			AV_PIX_FMT_RGBA,				//输出的像素格式
			SWS_BILINEAR,					//选择支持变化的算法，双线性插值
			0, 0, 0							//过滤器参数(不使用)
		);
		if (!yuv2rgb) {
			cerr << "sws_getCachedContext failed!" << endl;
			return;
		}

		unsigned char* data[1];
		data[0] = rgba;
		int lines[1] = { rgba_linesize };
		int re = sws_scale(yuv2rgb,
			yuv,			//输入数据
			yuv_linesize,	//输入输就行字节数 
			0,
			height,			//输入高度
			data,			//输出数据
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
	// YUV420P 平面存储 yyyy yyyy uu vv
	unsigned char *yuv[3] = { 0 };
	int yuv_linesize[3] = { width, width / 2, width / 2 };
	yuv[0] = new unsigned char[width * height];
	yuv[1] = new unsigned char[width * height / 4];
	yuv[2] = new unsigned char[width * height / 4];

	// RGBA交叉存储 RGBA RGBA
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
		// 读取RGBA数据
		ifs.read((char*)rgba, rgb_width * rgb_height * 4);
		if (ifs.gcount() == 0) break;

		// YUV转RGB
		rgba2yuv = sws_getCachedContext(
			rgba2yuv,						// 转换上下文，NULL新创建，非NULL判断与现有参数是否一致，一直直接返回，
											// 不一致先清理当前的数据再创建
			rgb_width, rgb_height,			// 输入宽高
			AV_PIX_FMT_RGBA,				//输入像素格式
			width, height,					//输出的宽高
			AV_PIX_FMT_YUV420P,				//输出的像素格式
			SWS_BILINEAR,					//选择支持变化的算法，双线性插值
			0, 0, 0							//过滤器参数(不使用)
		);
		if (!rgba2yuv) {
			cerr << "sws_getCachedContext failed!" << endl;
			return;
		}

		unsigned char* data[1];
		data[0] = rgba;
		int lines[1] = { rgba_linesize };
		int re = sws_scale(rgba2yuv,
			data,			//输入数据
			lines,	//输入输就行字节数 
			0,
			rgb_height,			//输入高度
			yuv,			//输出数据
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
	// 创建frame对象
	auto frame1 = av_frame_alloc();

	// 图像参数
	frame1->width = 400;
	frame1->height = 300;
	frame1->format = AV_PIX_FMT_ARGB;
	// 分配空间,QT采用16对齐方式，ffmpeg默认是32字节对齐，最小4字节对齐
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
			av_buffer_get_ref_count(frame1->buf[0]); // 线程安全
		cout << endl;
	}

	auto frame2 = av_frame_alloc();
	av_frame_ref(frame2, frame1);
	cout << "frame1 ref count = " <<
		av_buffer_get_ref_count(frame1->buf[0]); // 线程安全
	cout << endl;

	cout << "frame2 ref count = " <<
		av_buffer_get_ref_count(frame2->buf[0]); // 线程安全
	cout << endl;

	//引用计数-1，并将buff清零
	av_frame_unref(frame2);
	cout << "av_frame_unref(frame2)" << endl;
	cout << "frame1 ref count = " <<
		av_buffer_get_ref_count(frame1->buf[0]); // 线程安全
	cout << endl;

	//引用计数为1，直接删除buf空间，引用计数变为0
	av_frame_unref(frame1);

	// 是否frame对象空间，buf的引用计数减一
	// buf已经为空，只删除frame对象空间
	av_frame_free(&frame1);
	av_frame_free(&frame2);
	cout << "lll_test_swa_scale" << endl;
	// ffmpeg -i test.mp4 -s 400x300 400_300_25.yuv
	// 400x300 YUV 转 RGBA 800x600 并存到文件
	//YUVToRGBA();
	cout << "RGBA2YUV" << endl;
	RGBA2YUV();
    return 0;
}