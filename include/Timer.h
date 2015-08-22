#ifndef _TIMER_H_
#define _TIMER_H_


#include<functional>

class Timer
{
	typedef std::function< void ()> TimerCallback;
	public:
	explicit Timer( int val, int interval, TimerCallback cb);
	~Timer();
	void start();
	void stop();


	private:
	int m_val;
	int m_interval;
	TimerCallback m_cb;	
	bool  m_starting;
	int m_timerfd;
};




#endif

