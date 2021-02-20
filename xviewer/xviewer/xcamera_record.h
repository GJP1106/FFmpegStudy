#pragma once
#include <string>
#include "xtools.h"
class XCameraRecord : public XThread
{
public:
	void set_rtsp_url(std::string url) { rtsp_url_ = url; }
	void set_save_path(std::string s) { save_path_ = s; }
private:
	void Main();
	std::string rtsp_url_;
	std::string save_path_;		//´æ´¢µÄ¸ùÄ¿Â¼
};

