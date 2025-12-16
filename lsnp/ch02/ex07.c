#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/times.h>

int main(void)
{
    int i;
    time_t c_time;
    clock_t oldtime, newtime;
    struct tms oldtms, newtms;
    if((oldtime=times(&oldtms))==-1) {
        perror("old times");
        exit(1);
    }
    for(i=1; i<=99999999; i++)
    time(&c_time);
    if((newtime=times(&newtms))==-1) {
        perror("new times");
        exit(1);
    }
    printf("Real Time : %ld clocks\n", newtime-oldtime);
    printf("User mode Time : %ld clocks\n",  newtms.tms_utime-oldtms.tms_utime);
    printf("System mode Time : %ld clocks\n", newtms.tms_stime-oldtms.tms_stime);
    return 0;
}
