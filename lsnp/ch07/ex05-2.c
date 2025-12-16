#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
    int msgid,n;
    struct mymsgbuf{
        long m_type; // 타입 선언은 무조건 먼저 해주,이게 핵심
        char m_str[1024];
    } recvbuf;

    if((msgid=msgget(0x123400,IPC_CREAT|0666))==-1)
    {
        perror("msgget");
        exit(1);
    }
    while(1)
    {
        n=msgrcv(msgid,&recvbuf,1024, 1,0);
        recvbuf.m_str[n]='\0';
        printf(">> %s\n",recvbuf.m_str);
        if(!strncmp(recvbuf.m_str,"end", 3))
            break;
    }
    msgctl(msgid, IPC_RMID, 0); //강베 삭제 후 종료
    return 0;
}