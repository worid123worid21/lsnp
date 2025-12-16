#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    int fd = 3;
    if(close(fd)==-1){
        perror("close");
        exit(1);
    }
    return 0;
}