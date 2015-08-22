#include"Conf.h"
#include<stdio.h>
#include<stdlib.h>
#include<fstream>
#include<streambuf>
#include<sstream>
#include<iterator>
#include"MutexGuard.h"

#define MY_LOG_ERROR(msg) \
	do{\
		perror(msg);\
		exit(EXIT_FAILURE);\
	}while(0)


Conf * Conf::m_instance = NULL;
Mutex Conf::m_mutex;

void Conf::init_instance( const std::string & file )
{
	MutexGuard guard(m_mutex);
	if( m_instance == NULL )	
	{
		m_instance = new Conf( file.c_str() ) ;
	}
}

const std::string  Conf::operator[]( const std::string & query) const
{
	auto it =conf_map.find(query);
	if( it != conf_map.end())	
		return it->second;
	else
	{
		std::string ret="";	
		return ret;
	}
}


const Conf & Conf::get_instance()
{
	if( m_instance == NULL )
	{
		MY_LOG_ERROR(" Conf has not beeen initialized!");
	}
	return *m_instance;
}





Conf::Conf( const char * file)
{
	if( NULL == file)
		MY_LOG_ERROR("file");
	std::ifstream ifs( file );
	if( !ifs )
		MY_LOG_ERROR("Conf::Conf::ifstream");

	std::streambuf *pbuf = ifs.rdbuf();
	size_t size = pbuf->pubseekoff(0, std::ios::end, std::ios::in);
	pbuf->pubseekpos(0, std::ios::in);
	char * content = new char [size+1];
	if( NULL == content )
		MY_LOG_ERROR("new");
	content[size] = '\0';
	pbuf->sgetn( content, size);
	std::string content_str( content,size) ;

		

	std::string str1 = "<config>";
	std::string str2 = "</config>";
	size_t pos1 = content_str.find( str1 );
	size_t pos2 = content_str.find( str2 );
	if( pos1 == std::string::npos || pos2 == std::string::npos )
		MY_LOG_ERROR("std::string.find");

	std::string data_str( content, pos1+str1.size(), pos2-pos1-str1.size()) ;
	std::istringstream is( data_str);

	std::string key;
	std::string value;
	while( is>>key>>value)
    {
	conf_map[key] = value;
    }

	delete [] content;
	ifs.close();
}





