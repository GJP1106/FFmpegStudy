#pragma once
#include <thread>
#include <mutex>
// 日志级别 DEBUG INFO FATAL
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
// 获取当前时间
long long NowMs();

class XThread {
public:
	// 启动线程
	virtual void Start();

	// 停止线程(设置退出标志，等待线程退出)
	virtual void Stop();
protected:
	// 线程入口函数
	virtual void Main() = 0;
	// 标志线程退出
	bool is_exit_ = true;
	//  线程索引号
	int index_ = 0;
private:
	std::thread th_;
	std::mutex m_;
};
class XTools
{
};

