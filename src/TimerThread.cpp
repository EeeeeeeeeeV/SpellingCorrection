#include"TimerThread.h"
#include"Logger.h"

TimerThread::TimerThread( int val ,int interval, TimerThreadCallback cb): m_timer( val, interval, cb), m_starting(false)
{
	MY_LOG_DEBUG("TimerThread::TimerThread()");
}

TimerThread::~TimerThread()
{
	MY_LOG_DEBUG("TimerThread::~TimerThread()");
	stop();
}

void TimerThread::start()
{
	MY_LOG_DEBUG("TimerThread::start()");
	if( !m_starting )
	{
		if( 0 != pthread_create( &m_threadid, NULL, run_in_thread,this))
		{
			MY_LOG_ERROR("TimerThread::start() error");
		}
		m_starting = true;
	}
}


void TimerThread::stop()
{
	MY_LOG_DEBUG("TimerThread::stop()");
	if( m_starting )
	{
		m_timer.stop();
		m_starting = false;
	}
}


void *TimerThread::run_in_thread( void * arg)
{
	MY_LOG_DEBUG("TimerThread::run_in_thread()");
	TimerThread * pthread = static_cast<TimerThread*> (arg);
	pthread->m_timer.start();
	return NULL;
}





