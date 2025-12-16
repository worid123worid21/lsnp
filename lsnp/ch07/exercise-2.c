#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

typedef struct
{
    long m_type; // 타입 선언은 무조건 먼저 해주,이게 핵심
    char m_str[1024];
} mybuf;


int main(void)
{
    int msgid,n;

    char buf[1024];

    mybuf sendbuf, recvbuf;

    if((msgid=msgget(0x123400,IPC_CREAT|0666))==-1)
    {
            perror("msgget");
            exit(1);
    }
    while(1)
    {
        //수신 부분
        n=msgrcv(msgid,&recvbuf,1024, 1,0); //양방향 핵심
        recvbuf.m_str[n]='\0';
        printf(">> %s\n",recvbuf.m_str);
        if(!strncmp(recvbuf.m_str,"end", 3))
            break;

        //송신 부분
        printf(" Input 2--> ");
        fgets(buf, 1023, stdin);
        sendbuf.m_type=2;
        strcpy(sendbuf.m_str,buf);
        msgsnd(msgid,&sendbuf, strlen(sendbuf.m_str),0);
        if(!strncmp(buf,"end",3))
            break;
    }

    sleep(1);
    msgctl(msgid, IPC_RMID, 0); //강제 삭제 후 종료

    return 0;
}