#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main(void)
{
    char buffer[80];
    int fd;

    if((fd=open("duptest", O_RDONLY))==-1){
        perror("open");
        exit(1);
    }

    dup2(fd, 0);
    close(fd);
    printf("Please input a string --> ");
    fgets(buffer, 80, stdin);
    printf("You input the string : %s\n", buffer);

    return 0;
}