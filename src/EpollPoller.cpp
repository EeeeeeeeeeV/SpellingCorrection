#include"EpollPoller.h"
#include"Socket.h"
#include"SocketIO.h"
#include"Logger.h"
#include<errno.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<errno.h>
#include<stdlib.h>
#include<unistd.h>
#include<cstdlib>
#include<memory>
#include<utility>

/*
#define MY_LOG_ERROR( msg ) \
	do{ \
		perror( msg )	; \
		exit(EXIT_FAILURE); \
	}while(0)
*/


EpollPoller::EpollPoller( const std::string & ip, unsigned int port ):
	m_looping(false),
	m_events(1024)
{

	MY_LOG_DEBUG("EpollPoller::EpollPoller() ");

	m_listenfd = socket( AF_INET , SOCK_STREAM, 0 );
	if( -1 ==  m_listenfd )
	{
		MY_LOG_ERROR("EpollPoller::EpollPoller():socket() error ");
	}
	
	InetAddress addr( ip, port);
	Socket socketobj( m_listenfd );
	socketobj.set_nonblock();
	socketobj.set_reuse_addr(true);
	socketobj.set_reuse_port(true);
	socketobj.set_tcp_no_delay(false);
	socketobj.set_keep_alive(false);
	socketobj.bind(addr);
	socketobj.listen();

	m_epollfd = epoll_create( 1 );

	if( -1 == m_epollfd)
	{
		MY_LOG_ERROR("epoll_create1() error");
	}	

	add_readfd( m_listenfd);
}

EpollPoller::~EpollPoller()
{

	MY_LOG_DEBUG("EpollPoller::~EpollPoller() ");

	close(m_listenfd);
	close(m_epollfd);
}


void EpollPoller::loop( )
{

	MY_LOG_DEBUG("EpollPoller::loop() ");
	m_looping = true;
	while( m_looping )
	{
		wait_epollfd();
	}
	printf("epoll loop quit !");

}

void EpollPoller::unloop()
{
	MY_LOG_DEBUG("EpollPoller::unloop() ");
	m_looping = false;
}

void EpollPoller::add_readfd( int fd )
{

	MY_LOG_DEBUG("EpollPoller::add_epollin_fd() ");

	struct epoll_event ev;
	memset(&ev, 0, sizeof(ev));

	ev.data.fd = fd;
	ev.events = EPOLLIN | EPOLLET;

	if( -1 == epoll_ctl( m_epollfd, EPOLL_CTL_ADD, fd, &ev))
	{
		MY_LOG_ERROR("epoll_ctl() error: %s ", strerror( errno ));
	}


}

void EpollPoller::del_readfd( int fd)
{

	MY_LOG_DEBUG("EpollPoller::delete_fd() fd = %d " ,fd);

	struct epoll_event ev;
	ev.data.fd = fd;
	ev.events = EPOLLIN | EPOLLET;

	if( -1 == epoll_ctl( m_epollfd, EPOLL_CTL_DEL, fd, &ev))
	{
		MY_LOG_ERROR("epoll_ctl() error");
	}
}

void EpollPoller::wait_epollfd()
{
	MY_LOG_DEBUG("EpollPoller::wait_epollfd() ");

	int readynum =0;
	do
	{
		readynum = epoll_wait( m_epollfd, &*m_events.begin(), static_cast<int>( m_events.size()), 5000 );
	}while( readynum == -1 && errno == EINTR);

	if( readynum == -1)
	{
		MY_LOG_ERROR("epoll_wait() error");
	}
	else if( readynum == 0)
	{
		MY_LOG_DEBUG("epoll_wait() timeout ");
		return ;	
	}
	else
	{
		for( int idx=0; idx != readynum; idx++)
		{
			if( m_events[idx].events & EPOLLIN )
			{
				if( m_events[idx].data.fd == m_listenfd )	
				{
					handle_connection();
				}
				else 
				{
					m_messageCallback( m_events[idx].data.fd, shared_from_this() );
				}
			}

		}
	}
}


void EpollPoller::set_message_callback( EpollPollerCallback cb)
{
	MY_LOG_DEBUG("EpollPoller::set_message_callback( )");
	m_messageCallback = std::move( cb );
}


void EpollPoller::handle_connection(  )
{

	MY_LOG_DEBUG("EpollPoller::handle_connection( )");

	Socket sckobj( m_listenfd);
	std::string ip;
	int port = 0;
	int peerfd =0;  
	while( (peerfd = sckobj.accept( ip, port) ) >0 )
	{
		add_readfd( peerfd );

		MY_LOG_DEBUG("accept connection peerfd=%d, ip=%s, port=%d", peerfd, ip.c_str(), port);
	}
	if( peerfd == -1)
	{
		if( errno != EAGAIN
				&& errno != ECONNABORTED 
				&& errno != EPROTO 
				&& errno != EINTR )	
		{
			MY_LOG_ERROR("EpollPoller::handle_connection(): m_listenfd error %s ", strerror(errno));

		}

	}
}


