#pragma once
#include "xformat.h"
class XDemux : public XFormat
{
public:
	/* �򿪽��װ
	   ���װ��ַ֧��rtsp
	*/
	static AVFormatContext* Open(const char* url);

	// ��ȡһ֡����
	bool Read(AVPacket *pkt);

	bool Seek(long long pts, int stream_index);
};

