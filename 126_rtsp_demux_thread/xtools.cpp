#include "xtools.h"
#include <iostream>
#include <sstream>
using namespace std;

void MSleep(unsigned int ms)
{
	auto beg = clock();
	for (int i = 0; i < ms; i++) {
		this_thread::sleep_for(1ms);
		if ((clock() - beg) / (CLOCKS_PER_SEC / 1000) >= ms) {
			break;
		}

	}
}

long long NowMs()
{
	return clock() / (CLOCKS_PER_SEC / 1000);
}

// �����߳�
void XThread::Start()
{
	unique_lock<mutex> lock(m_);
	static int i = 0;
	i++;
	index_ = i;
	is_exit_ = false;
	// �����߳�
	th_ = thread(&XThread::Main, this);
	stringstream ss;
	ss << "XThread::Start()" << index_;
	LOGINFO(ss.str());
}
// ֹͣ�߳�
void XThread::Stop()
{
	stringstream ss;
	ss << "XThread::Stop() begin" << index_;
	LOGINFO(ss.str());
	is_exit_ = true;
	if (th_.joinable()) {		// �ж��߳��Ƿ���Եȴ�
		th_.join();				// �ȴ����߳��˳�
	}
	ss.str("");
	ss << "XThread::Stop() end" << index_;
	LOGINFO(ss.str());

}
