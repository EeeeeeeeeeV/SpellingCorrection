#ifndef _STRUTIL_H_
#define _STRUTIL_H_

#include<string>
#include<vector>

//处理UTF8字符

class StrUtil
{
	public:
	explicit StrUtil( const std::string & str);
//将字符串逐个字地拆开
	const std::vector<std::string> & split( );
	int edit_distance( const std::string & str );
	private:
	int edit_distance( const std::vector< std::string > & w1, const std::vector<std::string> & w2);
	std::string m_str;
	std::vector< std::string > m_letterVec;

};

#endif
