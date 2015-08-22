#ifndef _INDEX_H_
#define _INDEX_H_

#include"Mutex.h"
#include"Dict.h"
#include<unordered_map>

class Index
{
	public:
		static const Index & get_instance();
		static void init_instance( );
		const std::vector< size_t > & operator[]( const std::string & letter) const ;

	private:
		Index( const Dict & dict );
		std::unordered_map< std::string, std::vector< size_t > > m_indexTable;
		const std::vector< size_t > m_emptyVec;
		static Index * m_instance;
		static Mutex m_mutex;

};


#endif
