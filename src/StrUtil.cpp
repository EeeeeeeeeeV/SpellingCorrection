#include"StrUtil.h"
#include"Logger.h"
#include<string.h>
#include<stdio.h>

StrUtil::StrUtil( const std::string & str): m_str(str)
{
	MY_LOG_DEBUG("StrUtil::StrUtil()");
	if( m_str.empty())
	{
		return ; 	
	}
	for( size_t i=0; i < m_str.size(); i++)
	{
		if( m_str[i] >= 'A' &&  m_str[i] <= 'Z')	
		{
			m_str[i] =  m_str[i] - 'A' +  'a';
		}
	}

	size_t  j=0, i=1;
	for( ; i < m_str.size(); i++)		
	{
		if( ( m_str[i] & 0xC0 )  != 0x80)		
		{
			std::string s1 = m_str.substr( j, i-j );
			m_letterVec.push_back( s1 );
			j = i;
		}
	}
	std::string s1 = m_str.substr( j, i-j );
	m_letterVec.push_back( s1 );

}

const std::vector< std::string> & StrUtil::split( )
{
	MY_LOG_DEBUG("StrUtil::split()");
	return m_letterVec;
}


int StrUtil::edit_distance( const std::string & str)
{				
	MY_LOG_DEBUG("StrUtil::edit_distance( const std::string & )");
	StrUtil strobj2(str); 	
	int dis = edit_distance( m_letterVec, strobj2.split());
	return dis;
}

//计算编辑距离
int StrUtil::edit_distance( const std::vector< std::string > & w1, const std::vector< std::string> & w2)
{

	MY_LOG_DEBUG("StrUtil::edit_distance( const std::vector<> & , const std::vector<> & )");
	int len1 = w1.size();
	int len2 = w2.size();

	int ** arr =  new int *[len1+1] ;
	if( arr == NULL)
	{
		MY_LOG_ERROR("StrUtil::edit_distance( ) new error " );
	}
	memset( arr, 0, len1+1 );

	for( int i =0 ; i< len1+1 ; i++)	
	{
		int *  p = new int[len2+1] ;
		if( p == NULL)
		{
			MY_LOG_ERROR("StrUtil::edit_distance() new error");
		}
		memset( p, 0, len2+1);
		arr[i] = p;
	}

	for( int i=0; i < len2+1; i++ )
	{
		arr[0][i] = i;
	}
	for( int i=0; i< len1+1; i++)
	{
		arr[i][0] = i;
	}
//i为目标字符串位置
//j为原始字符串位置
//插入、删除的权重为1
//如果w1[i-1] == w2[j-1],则替换权重为0
//否则，替换的权重为2
	for( int i =1; i <len1+1; i++)
	{
		for( int j=1; j< len2+1; j++)	
		{
			int ins = arr[i-1][j] +1;
			int del = arr[i][j-1] +1;
			int sub_w = (w1[i-1] == w2[j-1]) ? 0 :2 ;
			int sub = arr[i-1][j-1] + sub_w;
			int m1 = ins < del ? ins : del;	
			int min = m1 < sub ? m1 : sub; 
			arr[i][j] = min;
			//printf("arr[i-1][j]=%d\n", arr[i-1][j]);
			//printf("arr[i][j-1]=%d\n", arr[i][j-1]);
		//	printf("arr[i-1][j-1]=%d\n", arr[i-1][j-1]);
		//	printf("ins = %d , del = %d , sub = %d , min = %d \n", ins, del, sub, min);
		}
	}

	int dis = arr[len1][len2];
	for( int i =0 ; i < len1+1 ; i++)	
	{
		delete[] arr[i];
	}
	delete[] arr;

	return dis;
}




