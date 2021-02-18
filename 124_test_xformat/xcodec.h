#pragma once
#include <mutex>
#include <vector>
struct AVCodecContext;
struct AVPacket;
struct AVFrame;
void PrintErr(int err);
///////////////////////////////////////////
//// ����ͽ���Ļ���
class XCodec
{
public:
	////////////////////////////////////////////////
	/// ��������������
	/// @para codec_id ������ID�ţ���Ӧffmpeg
	/// @return ����������ģ�ʧ�ܷ���nullptr
	static AVCodecContext* Create(int codec_id, bool is_encode);
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

	///////////////////////////////////////////////////////////////////////////////////
	/// ����AVCodecContext ����һ��AVFrame�� ��Ҫ�������ͷ�av_frame_free
	AVFrame* CreateFrame();
protected:
	AVCodecContext* c_ = nullptr;	//������������
	std::mutex mux_;				//��������������
};

