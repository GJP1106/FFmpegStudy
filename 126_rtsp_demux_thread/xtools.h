#pragma once
#include <thread>
#include <mutex>
// ��־���� DEBUG INFO FATAL
enum XLogLevel
{
	XLOG_TYPE_DEBUG,
	XLOG_TYPE_INFO,
	XLOG_TYPE_ERROR,
	XLOG_TYPE_FATAL
};
#define LOG_MIN_LEVEL XLOG_TYPE_DEBUG
#define XLOG(s, level) \
    if (level >= LOG_MIN_LEVEL) \
    std::cout << level << ":" << __FILE__ << ":" << __LINE__ << ":\n"\
    << s << std::endl;
#define LOGDEBUG(s) XLOG(s, XLOG_TYPE_DEBUG)
#define LOGINFO(s) XLOG(s, XLOG_TYPE_INFO)
#define LOGERROR(s) XLOG(s, XLOG_TYPE_ERROR)
#define LOGFATAL(s) XLOG(s, XLOG_TYPE_FATAL)

void MSleep(unsigned int ms);
// ��ȡ��ǰʱ��
long long NowMs();

class XThread {
public:
	// �����߳�
	virtual void Start();

	// ֹͣ�߳�(�����˳���־���ȴ��߳��˳�)
	virtual void Stop();
protected:
	// �߳���ں���
	virtual void Main() = 0;
	// ��־�߳��˳�
	bool is_exit_ = true;
	//  �߳�������
	int index_ = 0;
private:
	std::thread th_;
	std::mutex m_;
};
class XTools
{
};

