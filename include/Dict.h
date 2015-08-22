#ifndef _DICT_H_
#define _DICT_H_

#include<string>
#include<vector>
#include<utility>
#include"Mutex.h"


//字典类，单例模式

class Dict
{
	public:
		static const Dict & get_instance();
		static  void init_instance();
		
	private:

		friend class Index;
		friend class Task;
		std::vector< std::pair< std::string , size_t > > m_lexiconVec;
		Dict( const std::string & file);
		static	Dict * m_instance;
		static Mutex m_mutex;
};



#endif
