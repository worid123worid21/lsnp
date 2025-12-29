#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

void interval_handler(int sig)
{
    printf("<interval handler>called : %ld\n", time(NULL));
}

int main(void)
{
    struct sigaction act;
    struct itimerval itimer;
    act.sa_handler=interval_handler;
    sigfillset(&act.sa_mask);
    act.sa_flags=SA_RESTART;
    sigaction(SIGALRM, &act, NULL);

    printf("<main> Current Time : %ld\n", time(NULL));

    memset(&itimer, 0, sizeof(itimer));

    itimer.it_value.tv_sec=5;
    itimer.it_interval.tv_sec=2;
    setitimer(ITIMER_REAL, &itimer, NULL);
    
    while(1)
        pause();   // watingfor any signal
    return 0;
}