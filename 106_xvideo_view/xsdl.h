#pragma once
#include "xvideo_view.h"
struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;
class XSDL : public XVideoView
{
public:
	void Close() override;
	bool IsExit() override;
	/// ��ʼ����Ⱦ����
	/// @param w ���ڿ��
	/// @param h ���ڸ߶�
	/// @param fmt ���Ƶ����ظ�ʽ
	/// @param win_id ���ھ���� ���Ϊ�գ������´���
	/// @param �Ƿ񴴽��ɹ�
	bool Init(int w, int h, Format fmt = RGBA, void *win_id = nullptr) override;
	////////////////////////////////
    /// ��Ⱦͼ�� �̰߳�ȫ
    /// @param data ��Ⱦ�Ķ���������
    /// @param linesize һ�����ݵ��ֽ����� ����YUV420P����Yһ���ֽ���
    /// linesize <= 0 �͸��ݿ�Ⱥ����ظ�ʽ�Զ������С
    /// @return ��Ⱦ�Ƿ�ɹ�
    bool Draw(const unsigned char* data, int linesize = 0) override;
	virtual  bool Draw(
		const unsigned char* y, int y_pitch,
		const unsigned char* u, int u_pitch,
		const unsigned char* v, int v_pitch) override;
private:
	SDL_Window *win_ = nullptr;
	SDL_Renderer *render_ = nullptr;
	SDL_Texture *texture_ = nullptr;
};

