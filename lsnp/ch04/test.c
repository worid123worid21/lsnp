#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h> //close() 함수 등 사용을 위해
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h> 

// 입력 받은 거 출력
// int main(int argc, char const *argv[])
// {
//     for (int i = 1; i < argc; i++) {
//         printf("%s ", argv[i]);
//     }
//     printf("\n");
//     return 0;
// }

// 파일 내용 출력 
// int main(int argc, char const *argv[])
// {
    
//     if (argc != 2) {
//         printf("옳바른 인자 수가 아닙니다. 파일을 하나만 입력 하시오");
//         return -1;
//     }

//     int fd;
// 	struct stat fbuf;// stat 사용
// 	char* buffer;//

// 	if((fd=open(argv[1], O_RDONLY))==-1) {
// 		perror("open1");
// 		fprintf(stderr, "File Read Fail.....\n");
// 	}else{
// 		//printf("File Read Success!! fd= %d\n", fd);
		
// 		stat(argv[1], &fbuf);
// 		// buffer=(char*) malloc(fbuf.st_size);
// 		// read(fd, buffer, fbuf.st_size);

//         // 파일 크기 + '\0' 만큼 메모리 할당
//         buffer = (char *)malloc(fbuf.st_size + 1);
//         if (buffer == NULL) {
//             perror("malloc");
//             close(fd);
//             return -1;
//         }
//         // 파일 읽기
//         read(fd, buffer, fbuf.st_size);
//         buffer[fbuf.st_size] = '\0'; // 문자열 종료

//         printf("\n");
//         printf("%s", buffer);

//         free(buffer);

// 		close(fd);
// 	}

//     return 0;
// }

// 창호님
// int main(int argc, char const *argv[]) {
//     if (argc != 2) {
//         printf("파일 입력해\n");
//         exit(1);
//     }

//     int fd;
//     struct stat fstatbuf;
//     char* buf;

//     if((fd = open(argv[1], O_RDONLY)) == -1) {
//         perror("open: ");
//         fprintf(stderr, "File Read Faile....\n");
//     } else {
//         printf("File Read Success!! fd = %d\n", fd);

//         if(stat(argv[1],&fstatbuf)==-1) {
//             perror(argv[1]);
//             exit(1);
//         }

//         buf = (char*)malloc(fstatbuf.st_size * sizeof(char) + 1);
//         read(fd, buf, fstatbuf.st_size);
//         buf[fstatbuf.st_size] = '\0';
//         printf("%s\n", buf);
        
//         close(fd);
//         free(buf);
//     }


//     return 0;
// }

//강사님
#define MAX_SIZE    1024
int main(int argc, char*argv[])
{
    int fd, length;
    char buffer[MAX_SIZE] ;
    if(argc!=2) {
        printf("Usage: %s filename\n",argv[0]);
        exit(1);
    }
    struct stat fbuf;
    if(stat(argv[1],&fbuf )==1) {
        perror("stat");
        exit(1);
    }
    if(!S_ISREG(fbuf.st_mode)) {
        printf("%s is NOT Regular file\n", argv[1]);
        exit(1);
    }
    if((fd = open(argv[1], O_RDONLY))==-1)  {
        perror("open");
        exit(1);
    }
    printf("buffer : ");
    while(1) {
        length = read( fd, buffer, MAX_SIZE-1 );
        printf("%s", buffer);
        if(length < MAX_SIZE-1)
            break;
    }
    printf("\n");
    if(close(fd)==-1) {
        perror("close");
        exit(1);
    };
    return 0;
}