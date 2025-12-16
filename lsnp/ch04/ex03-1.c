#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
    int fd;
    char buffer[]="Hello, dup!!\n";

    if((fd=open("duptest", O_WRONLY|O_CREAT|O_TRUNC,0666))==-1){
        perror("open");
        exit(1);
    }

    close(1);
    dup(fd);
    close(fd);
    
    write(1,buffer,strlen(buffer));
    printf("This is dup() test file\n");
    return 0;
}