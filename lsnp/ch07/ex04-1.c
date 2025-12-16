#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // pipe() 함수 사용, dup() 함수 사용
#include <fcntl.h> // 파일 열기 시 플래그(예: O_RDONLY, O_WRONLY, O_CREAT) 등을 정의하여 저수준 파일 I/O
#include <sys/wait.h> // wait() 사용
#include <errno.h>
#include <sys/types.h> //mkfifo(), myfifo()
#include <sys/stat.h> //mkfifo(), myfifo()

int main(void) 
{
    int pd, n;
    char msg[1024];
    if(mkfifo("./myfifo", 0600)==-1) {
        if(errno!=EEXIST) {
            perror("mkfifo");
            exit(1);
        }
    }
    if((pd=open("./myfifo", O_WRONLY)) == -1) {
        perror("open");
        exit(1);
    }
    while(1) {
        printf("namedpipe1> ");
        fgets(msg, 1023, stdin);
        if(strncmp(msg,"quit",4)==0)
            break;
        if((n=write(pd, msg, strlen(msg)))==-1) {
            perror("write");
            exit(1);
        }
    }
    close(pd);
    return(0);
}