#ifndef _TIMERTHREAD_H_
#define _TIMERTHREAD_H_

#include"Timer.h"
#include<pthread.h>
#include<functional>

class TimerThread
{
	typedef std::function< void ()> TimerThreadCallback;
	public:
	TimerThread( int val, int interval, TimerThreadCallback cb);
	~TimerThread();
	void start();
	void stop();


	private:
	static void * run_in_thread(void * arg);
	Timer m_timer;
	bool m_starting;
	pthread_t m_threadid;

};


#endif
