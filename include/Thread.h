#ifndef _THREAD_H_
#define _THREAD_H_

#include"Cache.h"
#include<memory>
#include<pthread.h>

class Thread
{
	public:	
		typedef std::function< void ( Cache & ) > ThreadCallback;
	explicit Thread( ThreadCallback callback, Cache & cache);
		~Thread();
	
		void start();
		void join();
		Cache & get_cache();

	private:
		static void * run_in_thread( void *);
		ThreadCallback m_callback;
		pthread_t m_threadid;
		bool  m_starting;
		Cache m_cache;
};

#endif
