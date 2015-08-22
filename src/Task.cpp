#include"Task.h"
#include"StrUtil.h"
#include"SocketIO.h"
#include"Index.h"
#include"Dict.h"
#include<queue>
#include<set>
#include"Logger.h"

class Query 
{
	public:
	Query() = default;
	~Query() = default;
	int m_dis;
	std::pair< std::string, size_t > m_val;

};



void Task::execute( Cache & cache)
{
	MY_LOG_DEBUG("Task::execute()");
//到缓存中查找
	std::string qw = cache.query( m_queryWord);
//查找失败
	if( qw.empty())	
	{
		std::set< size_t  > idxset;					
		get_indexset( idxset);
		query( idxset );
		cache.add_element( m_queryWord, m_resultWord  );
	}	
	else
	{
		m_resultWord = qw;	
	}
//返回结果
	SocketIO sio( m_fd );
	sio.writen( m_resultWord.c_str(), m_resultWord.size());
}

void Task::get_indexset( std::set< size_t> & idxset)
{
	MY_LOG_DEBUG("Task::get_indexset()");

	const Index & index = Index::get_instance();
	StrUtil strobj( m_queryWord );
	const std::vector< std::string > & vec = strobj.split();
	for( const auto & kvp : vec )
	{
		for( const auto & v : index[kvp])	
		{
			idxset.insert(v);
		}
	}

	MY_LOG_DEBUG("Task::get_indexset() idxset.size()=%ld",idxset.size());
}


void Task::query( const std::set< size_t > & idxset )
{
	MY_LOG_DEBUG("Task::query()");
	const Dict & dict = Dict::get_instance();
	const auto & lexicon = dict.m_lexiconVec;
	StrUtil strobj( m_queryWord );

	auto fobj = []
		( const Query & lhs, const Query & rhs)
		->bool
		{
			if( lhs.m_dis != rhs.m_dis)
			{
				return lhs.m_dis > rhs.m_dis;
			}
			else
			{
				return lhs.m_val.second < rhs.m_val.second;	
			}
		};

	std::priority_queue< Query, std::vector< Query >, decltype(fobj)>  que(fobj);

	for( const auto & v : idxset)
	{
		int dis = strobj.edit_distance( lexicon[v].first );

		MY_LOG_DEBUG("Task::query(): { %s , %s } distance =  %d", m_queryWord.c_str(), lexicon[v].first.c_str() , dis);

		if( dis > 2)
		{
			continue;	
		}
		else 
		{
			MY_LOG_DEBUG("Task::query(): que.push() %s", lexicon[v].first.c_str() );
			Query q;
			q.m_dis = dis;
			q.m_val = lexicon[v] ;
			que.push( q );
		}
	}

	if( !que.empty())
	{
		m_resultWord = que.top().m_val.first;
		MY_LOG_DEBUG("queue is not empty, result = %s", m_resultWord.c_str());

	}
	else
	{
		MY_LOG_DEBUG("Task::query(): queue is empty");
		m_resultWord = m_queryWord;
	}
	return ;
}



