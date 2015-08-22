#include"Timer.h"
#include"Logger.h"
#include<unistd.h>
#include<sys/timerfd.h>
#include<string.h>
#include<poll.h>

Timer::Timer( int val, int interval, TimerCallback cb ):m_val( val), m_interval(interval), m_cb( cb), m_starting(false)
{

	MY_LOG_DEBUG("Timer::Timer()");
	m_timerfd = timerfd_create( CLOCK_REALTIME,0 );
	if( -1 == m_timerfd)
	{
		MY_LOG_ERROR("Timer::Timer() timerfd_create error");
	}
	
}

Timer::~Timer()
{
	MY_LOG_DEBUG("Timer::~Timer()");
	stop();
	close(m_timerfd);
}


void Timer::stop()
{
	MY_LOG_DEBUG("Timer::stop()");
	if( m_starting )
	{
		m_starting = false;
		struct itimerspec val;
		memset(&val, 0, sizeof(val));
		if(-1 == timerfd_settime( m_timerfd, 0, &val, NULL))
		{
			MY_LOG_ERROR("Timer::stop() timerfd_settime error");
		}
	}
}


void Timer::start()
{
	MY_LOG_DEBUG("Timer::start()");
	struct itimerspec val;
	memset(&val, 0 ,sizeof(val));
	val.it_value.tv_sec = m_val;
	val.it_interval.tv_sec = m_interval;
	if(-1 == timerfd_settime( m_timerfd, 0, &val, NULL))
	{
		MY_LOG_ERROR("Timer::start() timerfd_settime error");
	}

	struct pollfd pfd;
	pfd.fd = m_timerfd;
	pfd.events = POLLIN;

	m_starting = true;
	while(m_starting)
	{
		int ret = poll( &pfd, 1, 5000);
		if( -1 == ret )
		{
			if( errno == EINTR )
			{
				continue;	
			}
			else
			{
				MY_LOG_ERROR("Timer::start() poll() error ");
			}
		}
		else if( 0 == ret )
		{
			MY_LOG_DEBUG("poll time out ");
		}
		if( POLLIN == pfd.revents)
		{
			uint64_t cnt=0;  
			read( m_timerfd, &cnt, sizeof(cnt));
			MY_LOG_DEBUG("read timer: %d", cnt);
			if( m_cb)
			{
				m_cb();
			}
		
		}
	}
}
