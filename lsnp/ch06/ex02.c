#include <signal.h>
#include <unistd.h>
#include <stdio.h>


void handler(int signo){
    write(1, "Control_C\n", 10);
}

int main(void) {
    
    //signal(SIGINT, handler); //중간에 다른 신호가 들어오면 끊어지는 문제가 있음
    //signal(SIGINT, SIG_IGN);
    signal(SIGINT, SIG_DFL);

    while(1);
    return 0;
}