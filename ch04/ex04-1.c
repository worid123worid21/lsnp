// 문제 
// 입력: ./ex04-1 파일명  출력: ionde 값, 파일명
// 만약 디토리 파일이 아니면 "디렉토리 파일 아님" 출력하고 종료
// stat() 함수를 사용하기

// #include <stdio.h>
// #include <fcntl.h>
// #include <unistd.h> //close() 함수 등 사용을 위해
// #include <sys/stat.h>
// #include <sys/types.h>
// #include <dirent.h> // 디렉토리 opsndir() 등 사용을 위해
// #include <stdlib.h> // 추가

// int main(int argc, char const *argv[])
// {
//     /* code */
//     return 0;
// }

/*
stat("/etc/hosts", &fbuf);
buffer=(char*) malloc(fbuf.st_size);
read(fd, buffer, fbuf.st_size);
*/

#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    struct stat fbuf;

    if (argc != 2) {
        fprintf(stderr, "사용법: %s <파일명>\n", argv[0]);
        return 1;
    }

    // 파일 상태 얻기
    if (stat(argv[1], &fbuf) < 0) {
        perror("stat 오류");
        return 1;
    }

    // 디렉토리인지 확인
    if (S_ISDIR(fbuf.st_mode)) {
        printf("i-node: %ld, 파일명: %s\n", (long)fbuf.st_ino, argv[1]);
    } else {
        printf("디렉토리 파일 아님\n");
        return 1;
    }

    return 0;
}
