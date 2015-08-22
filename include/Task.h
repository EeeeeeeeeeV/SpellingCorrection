#ifndef _TASK_H_
#define _TASK_H_

#include<string>
#include<set>
#include"Cache.h"

class Task
{
	public:
		explicit Task() = default ;
		explicit Task( int fd , const std::string & str):m_fd(fd), m_queryWord( str ){}
		void execute( Cache & cache );
		void get_indexset( std::set< size_t > & idxset);
		void query( const std::set< size_t > & idxset);

	private:
		int m_fd;
		std::string m_queryWord;
		std::string m_resultWord;
};

#endif
