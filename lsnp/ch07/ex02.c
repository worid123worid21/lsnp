#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // pipe() 함수 사용, dup() 함수 사용
#include <sys/wait.h> // wait() 사용

#define MAXSIZE 4096

int main(void) {

    int pd[2];
    pid_t pid;
    int n;
    char buf[MAXSIZE];
    
    if( pipe(pd) == -1 ) {
        perror("pipe");
        exit(1);
    }

    switch(pid=fork()) {
        case -1:
            perror("fork");
            exit(1);
        break;
        case 0:
            close(pd[0]);
            strcpy(buf,"Hello, Parent!!");
            write(pd[1], buf, strlen(buf));
            exit(1);
        break;
        default:
            close(pd[1]);
            n=read(pd[0], buf, MAXSIZE);
            buf[n]='\0';
            close(pd[0]);
            printf("PARENT : %s from CHILD\n", buf);
            if(waitpid(pid,NULL, 0)==-1) {
                perror("waitpid");
                exit(1);
            }
        break;
        }
    return 0;
}