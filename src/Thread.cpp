#include"Thread.h"
#include"Logger.h"


Thread::Thread( ThreadCallback callback, Cache & cache ): m_callback( callback ), m_threadid(0), m_starting( false) , m_cache( cache)
{

	MY_LOG_DEBUG("Thread::Thread( ThreadCallback, Cache) ");
}

Thread::~Thread()
{
	MY_LOG_DEBUG("Thread::~Thread() ");
	if( m_starting)
	{
		pthread_detach( m_threadid)	;
	}
}

void Thread::start()
{
	MY_LOG_DEBUG("Thread::start() ");
	if( !m_starting)
	{
		if( 0!=pthread_create( & m_threadid, NULL, run_in_thread, this))
		{
			MY_LOG_ERROR("Thread::start() create thread failed!");
		}
		m_starting = true;
	}
	MY_LOG_DEBUG("thread id = %lu ", m_threadid);

}

void Thread::join()
{
	MY_LOG_DEBUG("Thread::join() ");
	if( m_starting)
	{
		pthread_join( m_threadid, NULL);
		m_starting = false;
	}
}

Cache & Thread::get_cache()
{
	MY_LOG_DEBUG("Thread::get_cache() ");
	return m_cache;
}


void * Thread::run_in_thread( void * arg)
{
	MY_LOG_DEBUG("Thread::run_in_thread() ");
	Thread * pthread = static_cast< Thread * >(arg);
	if( pthread->m_callback)
	{
		pthread->m_callback( pthread->get_cache())	;
	}

	return NULL;
}


