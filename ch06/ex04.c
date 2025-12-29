#define _POSIX_C_SOURCE 200809L //POSIX 표준의 2008년 9월 개정판(SUSv3의 확장) 기능을 활성화하는 매크로

#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void int_handler(int signo)
{
    int i;

    printf("int_handler() start.\nPlease waiting for 5 secs!!\n");
    for (i=0; i<5; i++ ){
        printf( "%d\n", i+1);
        sleep(1);
    }
    printf("int_handler() end\n");
}

int main(void)
{
    struct sigaction act;

    act.sa_handler=int_handler;
    sigfillset( &act.sa_mask);
    sigaction( SIGINT, &act, NULL);

    while(1 )  {
        printf("sigaction() : signal mask test\n");
        sleep( 1);
    }
}
