#include <iostream>
#include <fstream>
using namespace std;
extern "C"{ //指定函数是c语言函数，函数名不包含重载标注
//引用ffmpeg头文件
#include <libavcodec/avcodec.h>
#include <libavutil/frame.h>
#include <libswscale/swscale.h>
#include <libavutil/opt.h>
}
//预处理指令导入库
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
	/// 1、找到编码器 AV_CODEC_ID_HEVC(h265)
	auto codec = avcodec_find_encoder(codec_id);
	if (!codec) {
		cerr << "codec not find!" << endl;
		return;
	}
	/// 2、编码上下文
	auto c = avcodec_alloc_context3(codec);
	if (!c) {
		cerr << "avcodec_alloc_context3 failed!" << endl;
		return;
	}

	/// 3、设定上下文参数
	c->width = 400;
	c->height = 300;
	// 帧时间戳的时间单位  pts * time_base = 播放时间(秒)
	c->time_base = { 1, 25 };			// 分数	1 / 25

	c->pix_fmt = AV_PIX_FMT_YUV420P;	// 元数据像素格式，与编码算法相关
	c->thread_count = 16;				// 编码线程数，可以通过调用系统接口获取cpu核心数量

	// 预设编码器参数
	//c->max_b_frames = 0;  //B帧设为0，降低延时，增大空间
	//int optRet = av_opt_set(c->priv_data, "preset", "ultrafast", 0);  //最快速度
	//if (optRet != 0) {
	//	cout << "preset failed!" << endl;
	//}
	//optRet = av_opt_set(c->priv_data, "tune", "zerolatency", 0);  //零延时， h265不支持B帧
	//if (optRet != 0) {
	//	cout << "preset failed!" << endl;
	//}

	////////////////////////////////////////////////////////
	//// ABR 平均比特率
	int br = 400000; // 400kb
	// c->bit_rate = br;

	//////////////////////////////////////////////////////////
	//// CQP恒定质量  H264中的QP范围从0到51
	// x264默认23 ，效果较好 18
	// x265默认28 ， 效果较好 25
	//av_opt_set_int(c->priv_data, "qp", 18, 0);


	//////////////////////////////////////
	// 恒定比特率(CBR) 由于MP4不支持NAL填充，因此输出文件必须为(MPEG-2 TS)
	/*c->rc_min_rate = br;
	c->rc_max_rate = br;
	c->rc_buffer_size = br;
	c->bit_rate = br;
	av_opt_set(c->priv_data, "nal-hrd", "cbr", 0);*/

	////////////////////////////////////////////////////
	// 恒定速率因子 (CRF)
	av_opt_set_int(c->priv_data, "crf", 23, 0);
	////////////////////////////////////////////////////
    // 约束编码(VBV) Constrained Encoding
	av_opt_set_int(c->priv_data, "crf", 23, 0);
	c->rc_max_rate = br;
	c->rc_buffer_size = br * 2;
	/// 4.打开编码器上下文
	int re = avcodec_open2(c, codec, NULL);
	if (re != 0) {
		char buf[1024] = { 0 };
		av_strerror(re, buf, sizeof(buf) - 1);
		cerr << "avcodec_open2 faild!" << buf << endl;
		return;
	}
	cout << "avcodec_open2 success!" << endl;
	/// 创建好AVFrame空间 未压缩数据
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
	// 十秒视频，250帧
	for (int i = 0; i < 250; i++) {
		// 生成AVFrame 数据  每帧数据不同
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
		frame->pts = i;   // 显示时间
		// 发送未压缩帧到线程中压缩
		re = avcodec_send_frame(c, frame);
		if (re != 0) {

			break;
		}
		while (re >= 0) { //返回多帧
			// 接收压缩帧，一般前几次调用返回空(缓冲，立刻返回，编码未完成)
            // 编码是在独立的线程中
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
	/// 释放编码器上下文
	avcodec_free_context(&c);
}

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
	//RGBA2YUV();
	Encode(argc, argv[1]);
    return 0;
}