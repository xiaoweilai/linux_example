//实例三：信号阻塞及信号集操作

#include "signal.h"
#include "unistd.h"

static void my_op(int);

main()
{
        sigset_t new_mask,old_mask,pending_mask;

        struct sigaction act;

        sigemptyset(&act.sa_mask);

        act.sa_flags=SA_SIGINFO;

        act.sa_sigaction=(void*)my_op;

        if(sigaction(SIGRTMIN+10,&act,NULL))

                printf("install signal SIGRTMIN+10 error\n");

        sigemptyset(&new_mask);

        sigaddset(&new_mask,SIGRTMIN+10);

        if(sigprocmask(SIG_BLOCK, &new_mask,&old_mask))

                printf("block signal SIGRTMIN+10 error\n");

        sleep(10);

        printf("now begin to get pending mask and unblock SIGRTMIN+10\n");

        if(sigpending(&pending_mask)<0)

                printf("get pending mask error\n");

        if(sigismember(&pending_mask,SIGRTMIN+10))

                printf("signal SIGRTMIN+10 is pending\n");

        if(sigprocmask(SIG_SETMASK,&old_mask,NULL)<0)

                printf("unblock signal error\n");

        printf("signal unblocked\n");
        sleep(10);
}

 

static void my_op(int signum)

{

        printf("receive signal %d \n",signum);

}

 

//编译该程序，并以后台方式运行。在另一终端向该进程发送信号(运行kill -s 42 pid，SIGRTMIN+10为42)，查看结果可以看出几个关键函数的运行机制，信号集相关操作比较简单。