#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_NUMS 100
void new_op(int,siginfo_t*,void*);
static unsigned long ulnums = 0;
int main(int argc,char**argv)

{

    struct sigaction act;
    union sigval mysigval;
    int i;
    int sig;
    pid_t pid;
    char data[MAX_NUMS];
    memset(data,0,sizeof(data));

    for(i=0;i < MAX_NUMS;i++)
        data[i] = i  + '0';

    mysigval.sival_ptr=data;
    printf("argv[1]:%s\n", argv[1]);

    printf("argv[0]:%s\n", argv[0]);

    printf("atoi(argv[1]):%d\n", atoi(argv[1]));

    sig=atoi(argv[1]);
    pid=getpid();

    sigemptyset(&act.sa_mask);
    act.sa_sigaction=new_op;//三参数信号处理函数
    act.sa_flags=SA_SIGINFO;//信息传递开关，允许传说参数信息给new_op

    if(sigaction(sig,&act,NULL) < 0)
    {
        printf("install sigal error\n");
    }

    while(1)
    {
//        sleep(2);
        usleep(100);
        printf("wait for the signal\n");
        sigqueue(pid,sig,mysigval);//向本进程发送信号，并传递附加信息
    }

}

void new_op(int signum,siginfo_t *info,void *myact)//三参数信号处理函数的实现
{
    int i;



    for(i=0;i<MAX_NUMS;i++)
    {
        printf("%c ",(*( (char*)((*info).si_ptr)+i)));
    }
    printf("\n");

    printf("handle signal %d over;received no:%d\n",signum,     ++ulnums);
}


