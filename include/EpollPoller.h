#ifndef _EPOLLPOLLER_H_
#define _EPOLLPOLLER_H_

#include<vector>
#include<sys/epoll.h>
#include<string>
#include<memory>
#include<functional>
#include"ThreadPool.h"
#include"Message.h"


class EpollPoller ;
typedef std::shared_ptr<EpollPoller> EpollPollerPtr;

class EpollPoller : public std::enable_shared_from_this< EpollPoller>
{
	public:
		typedef std::function< void ( int , EpollPollerPtr  ) > EpollPollerCallback;

		explicit EpollPoller( const std::string & ip, unsigned int  port  );

		~EpollPoller();
		void add_readfd( int fd );
		void del_readfd( int fd );
		void handle_connection();
		void set_message_callback( EpollPollerCallback cb);
		void loop( );
		void unloop();

	private:
		void wait_epollfd( );
		int m_epollfd;
		int m_listenfd;
		bool m_looping;
		std::vector< struct epoll_event> m_events;
		EpollPollerCallback m_messageCallback;
};


#endif
