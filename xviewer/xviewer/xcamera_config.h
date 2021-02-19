#pragma once
#include <vector>
#include <mutex>
struct XCameraData
{
	char name[1024] = { 0 };
	char url[4096] = { 0 };			//摄像机主码流
	char sub_url[4096] = { 0 };		//摄像机辅码流
	char save_path[4096] = { 0 };	//视频录像存放目录
};
class XCameraConfig
{
public:
	// 唯一对象实例 单价模式
	static XCameraConfig* Instance() {
		static XCameraConfig xc;
		return &xc;
	}
	// 插入摄像机 线程安全
	void Push(const XCameraData& data);

	// 获取摄像头，摄像头索引 从0开始
	XCameraData GetCam(int index);

	// 修改摄像机数据
	bool SetCam(int index, const XCameraData& data);

	bool DelCam(int index);

	// 获取相机的数量
	// 失败返回0
	int GetCamCount();
	// 存储配置文件
	bool Save(const char* path);

	// 读取配置 存入cams_
	bool Load(const char* path);
private:
	XCameraConfig(){}			//构造私有， 单件模式
	std::vector<XCameraData> cams_;
	std::mutex mux_;
};

