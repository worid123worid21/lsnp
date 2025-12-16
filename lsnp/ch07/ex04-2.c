#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // pipe() 함수 사용, dup() 함수 사용
#include <fcntl.h> // 파일 열기 시 플래그(예: O_RDONLY, O_WRONLY, O_CREAT) 등을 정의하여 저수준 파일 I/O
#include <sys/wait.h> // wait() 사용
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(void)
{
    int pd, n = 0;
    char rcvmsg[1024];
    if(mkfifo("./myfifo", 0600)==-1) {
        if(errno!=EEXIST) {
            perror("mkfifo");
            exit(1);
        }
    }
    if((pd=open("./myfifo", O_RDONLY)) == -1 ) {
        perror("open");
        exit(1);
    }
    while((n=read(pd, rcvmsg, 1023))>0) {
        rcvmsg[n]='\0';
        write(1,"namedpipe2> ", 12);
        write(1, rcvmsg, n );
    }
    if(n==-1) {
        perror("read");
        exit(1);
    }
    write(1,"\n", 1 );
    close(pd);
    return(0);
}