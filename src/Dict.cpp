#include"Dict.h"
#include"Conf.h"
#include"Logger.h"
#include<string.h>
#include<errno.h>
#include<fstream>
#include"Conf.h"
#include"MutexGuard.h"

#include<sstream>

Dict* Dict::m_instance = NULL;
Mutex Dict::m_mutex;

//字典文件每行的格式：单词，词频


const Dict & Dict::get_instance()
{

	MY_LOG_DEBUG("Dict::get_instance()");
	if( m_instance == NULL )
	{
		init_instance();
	}
	return *m_instance;
}


void Dict::init_instance()
{
	MY_LOG_DEBUG("Dict::init_instance()");
	MutexGuard guard(m_mutex);
	if( m_instance == NULL )
	{
		const Conf & cf = Conf::get_instance();
		m_instance = new Dict( cf["dictfile"] );
	}
}



Dict::Dict( const std::string & file )
{
	MY_LOG_DEBUG("Dict::Dict()");
	std::ifstream ifs( file, std::ios::in | std::ios::binary );
	if( !ifs )
	{
		MY_LOG_ERROR( "Dicit::Dict() ifstream error %s", strerror(errno));
	}
	std::string line;
	std::string word;
	size_t fre;
	std::map< std::string ,size_t > wordMap;
	while( getline(ifs, line))
	{
		std::istringstream iss(line);
		iss>>word>>fre;
		wordMap[word] = fre;
	}
	for( const auto & v : wordMap )
	{
		m_lexiconVec.push_back(v);
	}

	MY_LOG_DEBUG("Dict::Dict(): total word = %d" , m_lexiconVec.size());
	ifs.close();
}

