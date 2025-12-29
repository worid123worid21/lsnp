#include<signal.h>
#include<unistd.h>
#include<stdio.h>

int main(void) 
{
    sigset_t set1, set2;
    sigfillset(&set1);
    sigemptyset(&set2);
    sigaddset(&set2, SIGINT);
    sigprocmask(SIG_BLOCK, &set1, NULL);    // 모든 시그널 블록 설정
    printf("signal block start \n");
    sleep(5); // 
    sigprocmask(SIG_UNBLOCK, &set2, NULL); // SIGINT 시그널 블록에서 제거
    while(1) {
        printf("Hello Signal\n");
    sleep(2);
    }
    return 0;
}