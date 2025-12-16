#define _POSIX_C_SOURCE 200809L //POSIX 표준의 2008년 9월 개정판(SUSv3의 확장) 기능을 활성화하는 매크로

#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int count=0;
void handler(int signo)
{
    count++;
}

int main(void)
{
    int i=0;
    struct sigaction act, oldact;

    act.sa_handler= handler;
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask,SIGINT);
    sigaddset(&act.sa_mask,SIGQUIT);

    act.sa_flags=SA_RESTART;
    sigaction(SIGINT,&act,&oldact); //기존 핸들러 받아오기

    while(i<10)     {
        printf("signal test\n");
        sleep(2);
        i++;
    }

    sigaction(SIGINT, &oldact, NULL);
    printf("count = %d\n", count);
    while(1);
}
