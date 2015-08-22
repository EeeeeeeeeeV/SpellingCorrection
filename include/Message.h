#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include<memory>
class EpollPoller ;
class ThreadPool;
typedef std::shared_ptr<EpollPoller> EpollPollerPtr;
typedef std::shared_ptr<ThreadPool> ThreadPoolPtr;

//读取socket，并添加任务到队列
void handle_message( int fd, EpollPollerPtr epoll_ptr, ThreadPoolPtr  threadpool_ptr );



#endif
