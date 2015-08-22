#include"Message.h"
#include"Logger.h"
#include"SocketIO.h"
#include"Task.h"
#include"ThreadPool.h"
#include"EpollPoller.h"

void handle_message( int fd , EpollPollerPtr epoll_ptr, ThreadPoolPtr  threadpool_ptr )
{
	MY_LOG_DEBUG("::handle_message()");

	SocketIO sio(fd);

	std::unique_ptr< char > buf( new char [1024]);
	if( !buf )
	{
		MY_LOG_ERROR("EpollPoller::handle_message: new error ");
	}

	ssize_t nread = 0;
	while( (nread = sio.readline( buf.get(), 1024)) > 0  )
	{
		MY_LOG_DEBUG("read: peerfd=%d, text={%s}, nread=%d", fd, buf.get(), nread);

		std::string text(buf.get(), nread -2 );

		MY_LOG_DEBUG("add tasking:" );

		Task task( fd, text);
		threadpool_ptr->add_task( task );
	}

	if(  nread == -1 && errno != EAGAIN  )
	{
		epoll_ptr->del_readfd(fd);
		MY_LOG_DEBUG(" remove peerfd=%d ",  fd);
	}

}



