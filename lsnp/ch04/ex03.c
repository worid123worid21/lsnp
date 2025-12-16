#include <stdio.h>
#include <fcntl.h>
#include <unistd.h> //close() 함수 등 사용을 위해
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h> // 추가

int main(void)
{
	int fd;

	struct stat fbuf;// stat 사용
	char* buffer;//

	if((fd=open("/etc/hosts", O_RDONLY))==-1) {
		perror("open1");
		fprintf(stderr, "File Read Fail.....\n");
	}else{
		printf("File Read Success!! fd= %d\n", fd);
		
		stat("/etc/hosts", &fbuf);//
		buffer=(char*) malloc(fbuf.st_size);//
		read(fd, buffer, fbuf.st_size);//

		close(fd);
	}
	if((fd=open("myhosts", O_WRONLY|O_CREAT|O_TRUNC, 0666))==-1) {
		perror("open2");
		fprintf(stderr, "File Create Fail.....\n");
	} else{
		printf("File Creat Success!! fd= %d\n", fd);
		write(fd, buffer, fbuf.st_size);
		close(fd);
	}
	return 0;
}