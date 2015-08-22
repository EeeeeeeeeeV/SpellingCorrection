#include"Logger.h"
#include"EpollPoller.h"
#include"ThreadPool.h"
#include"Dict.h"
#include"Index.h"
#include"Message.h"
#include"Daemon.h"
#include<cstdlib>
#include<stdio.h>
#include<iostream>
#include<functional>
#include<utility>
#include<memory>


#define CONF_FILE "/zrepo/code/SpellingCorrection/data/config"

int main()
{
	int start = clock();

	Conf::init_instance( CONF_FILE );

	my_daemon();

	Logger::init_instance();
	Dict::init_instance();
	Index::init_instance();

	printf("%.3lf second\n", double( clock()-start)/ CLOCKS_PER_SEC);

	Conf cf = Conf::get_instance();
	
	size_t queuesize = strtoul( cf["queuesize"].c_str() ,NULL, 0);
	size_t threadsnum = strtoul( cf["threadsnum"].c_str(), NULL, 0 ); 
	std::shared_ptr< ThreadPool > threadpool_ptr( new ThreadPool( queuesize, threadsnum ));		

	threadpool_ptr->start();

	unsigned short port = atoi( cf["port"].c_str() );
	std::shared_ptr< EpollPoller > epoll_ptr(new  EpollPoller( cf["ip"], port));

	epoll_ptr->set_message_callback( std::bind( &handle_message, std::placeholders::_1, std::placeholders::_2, threadpool_ptr ));

	epoll_ptr->loop();
	epoll_ptr->unloop();

	threadpool_ptr->stop();

	Logger::shut_down();

	return 0;

}
