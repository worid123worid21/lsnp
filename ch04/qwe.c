#include <stdio.h>
#include <sys/stat.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
    //struct stat 내부 구조체
    struct stat fbuf; // fbuf 변수로 선언

    if (argc != 2){ //인자 수가 2개가 아니면
        fprintf(stderr, "사용법: %s <파일명>\n", argv[0]); //표준애러 2 들어오면 출력문
        return 1;// 정상적 종료 0, 나머지는 오류값
    }

    if (stat(argv[1], &fbuf) < 0){ 
    // argv[1]에 있는 파일 이름을 사용해 stat() 함수로 파일 상태 정보를 조회하고,
    // 그 결과를 fbuf 구조체에 저장합니다.
        perror("stat 오류"); // **표준 오류(stderr)**로 "stat 오류"와 시스템 에러 메시지를 출력
        return 1; // 비정상 종료
    }

    //S_ISDIR() 디렉토리 파일이면 참 -> 참이면 true 반환
    if(S_ISDIR(fbuf.st_mode)){ 
    //S_ISDIR()는 st_mode 값을 검사하여 디렉토리 여부를 판단하는 표준 매크로
        printf("i-node: %ld, 파일명: %s\n", (long)fbuf.st_ino, argv[1]);
        // 디렉토리 파일이 참이면, inode value, argv[1] 값 출력
    } else {
        printf("디렉토리 파일 아님\n");
        return 1;
    }

    return 0;
}
