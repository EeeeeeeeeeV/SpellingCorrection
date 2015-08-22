#include"Cache.h"
#include"MutexGuard.h"
#include"Logger.h"
#include<sstream>


Cache::Cache(): m_mutex(), m_cacheMap() 
{
	MY_LOG_DEBUG("Cache::Cache()");
}

Cache::Cache( const Cache & rhs): m_mutex(), m_cacheMap( rhs.m_cacheMap )
{
	MY_LOG_DEBUG("Cache::Cache( const Cache & )");
}

void Cache::load_cache( const std::string & file)
{
	MY_LOG_DEBUG("Cache::load_cache()");
	MutexGuard guard(m_mutex);
	std::ifstream ifs( file , std::ios::in | std::ios::binary );
	if( !ifs)
	{
		MY_LOG_ERROR("Cache::load_cache() ifstream errror");
	}

	std::string line;	
	while( getline(ifs, line))
	{
		std::string key;
		std::string value;
		std::istringstream iss(line);
		iss>>key>>value;
		m_cacheMap[key] = value;
	}
	ifs.close();
}

void Cache::save_cache( const std::string & file)
{

	MY_LOG_DEBUG("Cache::save_cache()");
	std::ofstream ofs( file , std::ios::out | std::ios::binary );
	if( !ofs )
	{
		MY_LOG_ERROR("Cache::save_cache() ofstream error");
	}
	save_cache(ofs);
	ofs.close();
}

void Cache::save_cache( std::ofstream & ofs)
{
	MutexGuard guard(m_mutex);
	ofs.clear();
	ofs.seekp(0, std::ios::beg);
	if( !ofs)
	{
		MY_LOG_ERROR("Cache::save_cache() ofstream errror");
	}
	for( const auto & v : m_cacheMap)
	{
		ofs<<v.first<<"    "<<v.second<<"\n";
	}
}


std::string Cache::query( const std::string & rhs) 
{
	MY_LOG_DEBUG("Cache::query() %s", rhs.c_str());
	MutexGuard guard(m_mutex);
	auto it = m_cacheMap.find( rhs );
	if( it != m_cacheMap.end())
	{
		MY_LOG_DEBUG("Cache::query() %s found ", rhs.c_str());
		return it->second;	
	}
	else
	{
		MY_LOG_DEBUG("Cache::query() %s not found ", rhs.c_str());
		std::string not_found="";
		return not_found;	
	}

}



Cache & Cache::operator+=( const Cache & rhs)
{
	MY_LOG_DEBUG("Cache::operator+=()");
	MutexGuard guard(m_mutex);
	for( const auto & v : rhs.m_cacheMap )	
	{
		m_cacheMap[v.first] = v.second;
	}
	return *this;
}

Cache & Cache::operator=( const Cache & rhs)
{
	MY_LOG_DEBUG("Cache::operator=()");
	MutexGuard guard(m_mutex);
	m_cacheMap.clear();
	for( const auto & v : rhs.m_cacheMap )	
	{
		m_cacheMap[v.first] = v.second;
	}
	return *this;
}


void Cache::add_element( const std::string & key, const std::string & value)
{
	m_cacheMap[key] = value;

}

