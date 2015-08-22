#include"Index.h"
#include"Dict.h"
#include"StrUtil.h"
#include"MutexGuard.h"
#include<set>
#include"Logger.h"

Index * Index::m_instance = NULL;
Mutex   Index::m_mutex ;


void Index::init_instance( )
{
	MY_LOG_DEBUG("Index::init_instance()");
	
	MutexGuard guard(m_mutex);
	if( m_instance == NULL )
	{
		const Dict & dict = Dict::get_instance();
		m_instance = new Index( dict );
	}
}

const Index & Index::get_instance()
{
	MY_LOG_DEBUG("Index::get_instance()");

	if( m_instance == NULL)
	{
		init_instance();
	}
	return *m_instance;
}


Index::Index( const Dict & dict): m_emptyVec()
{
	MY_LOG_DEBUG("Index::Index()");
	 const std::vector< std::pair< std::string, size_t > > & lexicon = dict.m_lexiconVec;

	MY_LOG_DEBUG("Index::Index() total lexicon = %ld ", lexicon.size());

	for( size_t idx ; idx < lexicon.size() ; idx++  )
	{
		StrUtil  strobj( lexicon[idx].first );
		const std::vector< std::string > & vec = strobj.split();
		std::set< std::string > letterSet;
		for( const auto & v : vec  )	
		{
			letterSet.insert(v);
		}
		for( const auto & v : letterSet )
		{
			m_indexTable[v].push_back(idx);
		}

		MY_LOG_DEBUG("Index::Index() %s: %d letter, idx=%ld ", lexicon[idx].first.c_str() ,letterSet.size(), idx);
	}
}


const std::vector< size_t > & Index::operator[](const std::string & letter ) const 
{
	MY_LOG_DEBUG("Index::operator[]");
	auto it = m_indexTable.find( letter );
	if( it != m_indexTable.end())
	{

		MY_LOG_DEBUG("Index::operator[] %s, match %ld word", letter.c_str(), it->second.size()  );
		return it->second ;	
	}
	else
	{
		MY_LOG_DEBUG("Index::operator[] %s, match none ");
		return m_emptyVec;
	}
}


