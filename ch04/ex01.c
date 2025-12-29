#include <stdio.h>
#include <fcntl.h>
#include <unistd.h> //close() 함수 등 사용을 위해
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h> // 추가
#include <string.h>

int main(int argc, char*argv[])
{
    char buffer[1024];
    int fd, n;
    if(argc != 2) {
        fprintf(stderr, "Usage : ex01 device_filename\n");
        exit(1);
    }

    if((fd=open(argv[1],O_RDWR))==-1) {
        perror("open");
        exit(1);
    }
    strcpy(buffer, "Hello, Terminal\n");
    n=strlen(buffer);
    write(fd,buffer,n);
    close(fd);
}