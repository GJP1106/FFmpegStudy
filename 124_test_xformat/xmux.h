#pragma once
#include "xformat.h"
////////////////////
/// ý���װ
class XMux : public XFormat
{
public:
	/// �򿪷�װ
	static AVFormatContext *Open(const char* url);

	bool WriteHead();
	bool Write(AVPacket* pkt);
	bool WriteEnd();
};

