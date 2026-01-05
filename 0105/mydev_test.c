//=======================================
// gpioled_test.c
//
//=======================================
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() 
{
 	int fd;
	char buf;
 
 	fd = open("/dev/mydev", O_RDWR);
	if(fd == -1) {
 		perror("open");
		exit(-1);
 	}

	read(fd, &buf, 1);
 	write(fd, &buf, 1);

	close(fd);

 	return 0;
}