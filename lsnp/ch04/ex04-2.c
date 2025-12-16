#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    struct stat fbuf;
    DIR *dp;
    struct dirent *d;

    if (argc != 2) {
        fprintf(stderr, "사용법: %s <디렉토리명>\n", argv[0]);
        return 1;
    }

    // stat()으로 파일 정보 가져오기
    if (stat(argv[1], &fbuf) < 0) {
        perror("stat 오류");
        return 1;
    }

    // 디렉토리 여부 확인
    if (!S_ISDIR(fbuf.st_mode)) {
        printf("디렉토리 파일 아님\n");
        return 1;
    }

    // 디렉토리의 자기 i-node 출력
    printf("디렉토리 i-node: %ld, 이름: %s\n\n", (long)fbuf.st_ino, argv[1]);

    // 디렉토리 열기
    dp = opendir(argv[1]);
    if (dp == NULL) {
        perror("opendir 오류");
        return 1;
    }

    printf("-> 디렉토리 내부 목록:\n");

    // 디렉토리 내부 항목 읽기
    while ((d = readdir(dp)) != NULL) {
        printf("  이름: %-15s  i-node: %lu\n",
               d->d_name,
               (unsigned long)d->d_ino);
    }
    

    // 디렉토리 닫기
    closedir(dp);

    return 0;
}