#pragma once
#include <vector>
#include <mutex>
struct XCameraData
{
	char name[1024] = { 0 };
	char url[4096] = { 0 };			//�����������
	char sub_url[4096] = { 0 };		//�����������
	char save_path[4096] = { 0 };	//��Ƶ¼����Ŀ¼
};
class XCameraConfig
{
public:
	// Ψһ����ʵ�� ����ģʽ
	static XCameraConfig* Instance() {
		static XCameraConfig xc;
		return &xc;
	}
	// ��������� �̰߳�ȫ
	void Push(const XCameraData& data);

	// ��ȡ����ͷ������ͷ���� ��0��ʼ
	XCameraData GetCam(int index);

	// �޸����������
	bool SetCam(int index, const XCameraData& data);

	bool DelCam(int index);

	// ��ȡ���������
	// ʧ�ܷ���0
	int GetCamCount();
	// �洢�����ļ�
	bool Save(const char* path);

	// ��ȡ���� ����cams_
	bool Load(const char* path);
private:
	XCameraConfig(){}			//����˽�У� ����ģʽ
	std::vector<XCameraData> cams_;
	std::mutex mux_;
};

