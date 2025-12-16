#include <stdio.h>
#include <sys/stat.h> // 파일 정보 관련 구조체
#include <sys/types.h> // 기본 시스템 데이터 타입 정리
#include <dirent.h> // 디렉토리 관련
#include <stdlib.h> 

int main(int argc, char const *argv[])
{
	struct stat fbuf;
	DIR *dp; //typedef struct __dirstream DIR
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
	printf("디렉토리 i-node: %ld, name: %s\n\n", (long)fbuf.st_ino, argv[1]);

	// 디렉토리 열기
	dp = opendir(argv[1]); 
	// opendir 해당 스트림 포인터 반환
	// error면 NULL pointer 반환
	if (dp = NULL){
		perror("opendir 오류");
		return 1;
	}

	printf("-> 디렉토리 내부 목록:\n");

	// 디렉토리 내부 항목 읽기
	while ((d = readdir(dp)) != NULL){
	 // readdir(dp): 디렉토리 스트림 dp에서 다음 항목을 읽어 d에 저장.
	 // 더 이상 읽을 항목이 없으면 NULL을 반환하여 반복문 종료.
		printf(" 이름: %-15s i-node: %lu\n", d->d_name, (unsigned long)d->d_ino);
	}

	//디렉토리 닫기
	closedir(dp);

	return 0;
}