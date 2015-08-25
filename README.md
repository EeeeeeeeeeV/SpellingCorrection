# SpellingCorrection
本项目实现了一个简单的拼写纠错系统，加入了线程缓存和进程缓存机制。

#特点
最短编辑距离，Epoll，线程池，单例模式，TCP，STL，Log4cpp，C++11

#文件架构
```
./
├── bin 				执行文件目录
│   └── main
├── data
│   ├── cachefile 		cache缓存文件
│   ├── config 			配置文件
│   ├── dictfile 		字典文件
│   └── logcfg 			Log4cpp配置文件
├── include
│   ├── Cache.h 		Cache类
│   ├── Condition.h 	封装Linux条件变量
│   ├── Conf.h 			配置类，单例模式
│   ├── Daemon.h 		守护进程函数
│   ├── Dict.h 			字典类
│   ├── EpollPoller.h 	Epoll类
│   ├── Index.h 		字典索引类，根据字母/单个字建立字典的索引
│   ├── InetAddress.h 	Socket地址类
│   ├── Logger.h 		封装Log4cpp作为日志系统
│   ├── Message.h 		EpollPoller类的回调函数，读取Socket信息，添加任务到线程池
│   ├── MutexGuard.h 	临界区的进入和退出类
│   ├── Mutex.h 		RAII方法封装Linux mutex
│   ├── Socket.h 		socket设置类
│   ├── SocketIO.h 		封装了socket的I/O操作
│   ├── StrUtil.h 		字符串处理类，计算最短编辑距离
│   ├── Task.h 			任务类，执行纠错工作
│   ├── Thread.h 		线程类
│   ├── ThreadPool.h 	线程池类
│   ├── Timer.h 		定时器类
│   └── TimerThread.h 	Cache定时更新线程类
├── lib
│   └── liblog4cpp.so 	log4cpp动态库
├── log 				日志目录
│   └── log
├── Makefile
├── obj
├── src 				源文件
│   ├── Cache.cpp
│   ├── Condition.cpp
│   ├── Conf.cpp
│   ├── Daemon.cpp
│   ├── Dict.cpp
│   ├── EpollPoller.cpp
│   ├── Index.cpp
│   ├── InetAddress.cpp
│   ├── Logger.cpp
│   ├── main.cpp
│   ├── Message.cpp
│   ├── Mutex.cpp
│   ├── MutexGuard.cpp
│   ├── Socket.cpp
│   ├── SocketIO.cpp
│   ├── StrUtil.cpp
│   ├── Task.cpp
│   ├── Thread.cpp
│   ├── ThreadPool.cpp
│   ├── Timer.cpp
│   └── TimerThread.cpp
└── test
    ├── cli.cpp 		测试程序源文件
    ├── client.o
    ├── Makefile
    ├── SocketIO.cpp
    └── SocketIO.h

```
#程序执行流程：


初始化：
1. 
Begin ——> Load Configuration ——> Load Dictionary ——> Build Indexes ——>  ThreadPool Start 
2. 
Socket ——> EpollPool Looping

查询阶段：
Query Words ——> Epoll ——> Task ——> Word Set ——> MIN Edit Distance  ——> Result ——> Add Cache ——> return result;

Cache更新机制：
Cache分为线程Cache和进程Cache, 线程Cache定时同步到进程Cache，进程Cache定时更新线程Cache并把Cache写入磁盘

#样例
```
[usr1@localhost SpellingCorrection]$ ./test/client.o 
input : xello
result: hello

input : applee
result: apple

input : 氯化钠米
result: 氯化钠

input : 息息相管
result: 息息相关

input : 

```

#参考：
http://52opencourse.com/138/spelling-correction
http://www.ruanyifeng.com/blog/2012/10/spelling_corrector.html

