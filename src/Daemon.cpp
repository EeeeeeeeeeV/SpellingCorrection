#include"Daemon.h"
#include"Logger.h"
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/time.h>
#include<sys/resource.h>
#include<signal.h>
#include<string.h>
#include<errno.h>



void my_daemon(void)
{

	int pid = fork();
	if( pid < 0 )
	{
		MY_LOG_ERROR("daemon fork error %s ", strerror(errno))	;
		exit(0);
	}
	else if( pid >0)
	{
		exit(0)	;
	}

	struct sigaction act;
	act.sa_handler = SIG_IGN;
	sigemptyset( &act.sa_mask);
	act.sa_flags = 0;
	sigaction( SIGHUP, &act,NULL);

	pid = setsid();
	if( pid == -1)
	{
		MY_LOG_ERROR(" setsid error ")	;
	}

	pid = fork();
	if( pid < 0 )
	{
		MY_LOG_ERROR("daemon fork error %s ", strerror(errno))	;
		exit(0);
	}
	else if( pid >0)
	{
		exit(0)	;
	}

	struct rlimit rlmt;	
	getrlimit( RLIMIT_NOFILE, &rlmt);
	if( rlmt.rlim_max == RLIM_INFINITY)
	{
		rlmt.rlim_max = 1024;	
	}
	for( size_t i=0; i<rlmt.rlim_max; i++)
	{
		close(i);
	}

	int fd0 = open("/dev/null", O_RDWR);
	int fd1 = dup(fd0);
	int fd2 = dup(fd0);
	if( fd0 !=0 || fd1 !=1 || fd2 != 2)
	{
		MY_LOG_ERROR("fd error fd0 = %d, fd1 = %d, fd2 = %d ", fd0, fd1, fd2);
		exit(0);
	}

	chdir("/");
	umask(0);

}

