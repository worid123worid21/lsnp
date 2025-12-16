#include<stdio.h>
#include<unistd.h>
#include<signal.h>

int main( void)
{
    sigset_t sigset;
    sigfillset(&sigset);
    sigprocmask(SIG_BLOCK, &sigset, NULL); // 특정 시그널를 기다리는 함수, NULL(0) 으로 등록된 함수만 작동.
    // 나머지는 무시, 다른 함수들은 "1"로 등록
    sigfillset(&sigset);
    sigdelset(&sigset, SIGINT);
    sigsuspend(&sigset);
    return 0;
}