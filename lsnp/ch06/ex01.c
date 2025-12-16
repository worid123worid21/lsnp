#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>

//#define _POSIX_C_SOURCE 200809L

int main(void) {
    sigset_t set;

    sigfillset(&set);
    sigdelset(&set, SIGINT);
    sigprocmask(SIG_SETMASK, &set, NULL);

    while(1);
    return 0;
}