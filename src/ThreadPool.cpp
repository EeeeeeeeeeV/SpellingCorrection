#include"ThreadPool.h"
#include"Thread.h"
#include"MutexGuard.h"
#include"Task.h"
#include"Logger.h"
#include<utility>
#include<memory>
#include<functional>


ThreadPool::ThreadPool(size_t queue_size, size_t threads_num )
: m_mutex(),  
  m_empty( m_mutex), 
  m_full( m_mutex ), 
  m_queueSize( queue_size), 
  m_threadNum( threads_num ), 
  m_starting(false),
  m_timerthread( 5, 5, 
		  std::bind(&ThreadPool::update_cache, this))
{
	const Conf & cf = Conf::get_instance();
	m_cache.load_cache( cf["cachefile"] );
	MY_LOG_DEBUG("ThreadPool::ThreadPool()");

}

void ThreadPool::start()
{

	MY_LOG_DEBUG("ThreadPool::start()");
	if( m_starting == false)
	{
		for( size_t idx =0; idx != m_threadNum; idx++)
		{
			m_threadsVec.push_back( 
					std::unique_ptr< Thread > (
						new Thread( std::bind( &ThreadPool::run_in_thread, this, std::placeholders::_1 ), 
							m_cache
							) ));
		}

		m_starting = true;

		for( auto & v : m_threadsVec)
		{
			v->start();
		}
		m_timerthread.start();
			
	}
}


void ThreadPool::stop()
{

	MY_LOG_DEBUG("ThreadPool::stop()");

	if( m_starting == false )
	{
		return ;	
	}
	else 
	{
		m_starting = false;
		m_full.notify_all();
		m_empty.notify_all();
		for( auto & v : m_threadsVec )
		{
			v->join();
		}
		m_timerthread.stop();
		m_threadsVec.clear();
	}

}


void ThreadPool::add_task( Task & task )
{
	MutexGuard guard( m_mutex);
	while( m_starting  &&  m_taskQueue.size() >= m_queueSize)
	{
		m_empty.wait();
	}
	if( m_starting)
	{
		m_taskQueue.push( task );
		m_full.notify();
		MY_LOG_DEBUG("ThreadPool::add_task()");

	}	
}

Task ThreadPool::get_task()
{
	MutexGuard guard( m_mutex );
	while( m_starting && m_taskQueue.empty())
	{
		m_full.wait();
	}
	if( m_starting )
	{
		Task task = m_taskQueue.front();
		m_taskQueue.pop();
		m_empty.notify();

		MY_LOG_DEBUG("ThreadPool::get_task()");

		return task;
	}
	Task empty_task;
	return empty_task; 
}


void ThreadPool::run_in_thread(  Cache & cache)
{

	MY_LOG_DEBUG("ThreadPool::run_in_thread()");

	while( m_starting )	
	{
		Task task = get_task();
		if( m_starting)
		{
			task.execute( cache );
		}
	}

}

void ThreadPool::update_cache()
{
	MY_LOG_DEBUG("ThreadPool::update_cache()");
	const Conf & cf = Conf::get_instance();
	std::ofstream ofs( cf["cachefile"], std::ios::out | std::ios::binary)	;

	if( !ofs )
	{
		MY_LOG_ERROR("ThreadPool::update_cache() ofstream error ");
	}

	if(m_starting)
	{
		for( size_t i=0; i < m_threadsVec.size(); i++)	
		{
			m_cache += m_threadsVec[i]->get_cache();
		}
		for( size_t i=0; i < m_threadsVec.size(); i++)	
		{
			m_threadsVec[i]->get_cache() = m_cache;
		}
		m_cache.save_cache(ofs);
	}
	ofs.close();
}



ThreadPool::~ThreadPool()
{
	MY_LOG_DEBUG("ThreadPool::~ThreadPool()");
	stop();

}



