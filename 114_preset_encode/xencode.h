#pragma once
#include <mutex>
#include <vector>
struct AVCodecContext;
struct AVPacket;
struct AVFrame;
class XEncode
{
public:
	////////////////////////////////////////////////
	/// ��������������
	/// @para codec_id ������ID�ţ���Ӧffmpeg
	/// @return ���������ģ�ʧ�ܷ���nullptr
	static AVCodecContext* Create(int codec_id);
	////////////////////////////////////////////////
    /// ���ö���ı����������� �����Ĵ��ݵ������У���Դ��XEncodeά��
	/// �������̰߳�ȫ
	/// @para c������������ ���c_��Ϊnullptr������������Դ
	void set_c(AVCodecContext* c);

	//////////////////////////////////////////////
	/// ���ñ���������̰߳�ȫ
	bool SetOpt(const char* key, const char* val);
	bool SetOpt(const char* key, int val);

	/////////////////////////////////////////////////
	/// �򿪱����� �̰߳�ȫ
	bool Open();

	////////////////////////////////////////////////
	/// �������ݣ��̰߳�ȫ ÿ���´���AVPacket
	/// @para frame �ռ����û�ά��
	/// @return ʧ�ܷ���nullptr�����ص�AVPacket�û���Ҫͨ��av_packet_free ����
	AVPacket *Encode(const AVFrame* frame);

	///////////////////////////////////////////////////////////////////////////////////
	/// ����AVCodecContext ����һ��AVFrame�� ��Ҫ�������ͷ�av_frame_free
	AVFrame* CreateFrame();

	///////////////////////////////////////////////////////////////
	//�������б��뻺����AVPacket
	std::vector<AVPacket *> End();
private:
	AVCodecContext* c_ = nullptr;	//������������
	std::mutex mux_;				//��������������
};

