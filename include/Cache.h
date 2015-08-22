#ifndef _CACHE_H_
#define _CACHE_H_

#include"Mutex.h"
#include<string>
#include<unordered_map>
#include<fstream>


class Cache
{
	public:
		explicit Cache();
		Cache( const Cache & rhs);
		void load_cache( const std::string & file );
		void save_cache( const std::string & file );
		void save_cache( std::ofstream & ofs);
		std::string query( const std::string & rhs)	;
		Cache & operator+=( const Cache & rhs);
		Cache & operator=( const Cache & rhs);
		void add_element( const std::string & key, const std::string & value );

	private:
		Mutex m_mutex;
		std::unordered_map<std::string, std::string > m_cacheMap;


};

#endif
