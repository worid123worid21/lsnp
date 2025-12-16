#include <stdio.h>
#include<termio.h>
#include <stdlib.h>
#include <string.h>
#define CR  '\012'

#define MAX_PASS 128


int main(int argc, char *argv[])
{
    struct termio tbuf, oldtbuf;
    char ch;

	char passwd[MAX_PASS];
	int i = 0;

    if(ioctl(0, TCGETA, &tbuf) == -1) {     // 현재 터미널 모드
        perror("ioctl");
        exit(1);
    }

    oldtbuf=tbuf;
    //tbuf.c_lflag&= ~ICANON;             // ICANON 모드 해제 --> RAW 모드
    tbuf.c_lflag&= ~ECHO; 
	tbuf.c_cc[VMIN] = 1;                   // 입력 문자 최소 수
    tbuf.c_cc[VTIME] = 0;                   // 입력 완료 시간
    
    if(ioctl(0, TCSETAF, &tbuf)==-1) {
        perror("ioctl");
        exit(1);
    }

    while(1)   {
    	ch=getchar();
	
    	if(ch==CR)
        	break;
			// if(int i=0; i<argv; i++){
			// 	printf('*');
			// }
        
			//printf("*"); // 문자 출력
		if (i < MAX_PASS - 1) {
            passwd[i++] = ch;
            printf("*");  // print asterisk
            fflush(stdout);
        }
		
    }

	passwd[i] = '\0'; // Null terminate
	printf("\n");
    printf("Password entered: %s\n", passwd);

    if(ioctl(0, TCSETAF, &oldtbuf)==-1) {   // 원래 모드로 재 설정
        perror("iosddajasdfctl");
        exit(1);
    }
    
    return 0;
}
