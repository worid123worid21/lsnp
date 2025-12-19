#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <sys/select.h>     // 추가

#define MAXDATASIZE 1024 //수정

void chatting(int sd);  

int main(int argc, char const *argv[])
{
    int sockfd, numbytes;
    socklen_t addr_len;
    char buf[MAXDATASIZE];
    struct hostent* he;
    struct sockaddr_in server_addr;

    if(argc != 2){
        fprintf (stderr, "Usafe : client hostname \n");
        exit(1);
    }
    if((he = gethostbyname(argv[1]))==NULL){
        perror("gethostbyname");
        exit(1);
    }
    if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1){
        perror("socket");
        exit(1);
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(60000);
    server_addr.sin_addr = *((struct in_addr*)he->h_addr);
    memset(&(server_addr.sin_zero), '\0', 8);

    printf("connect to [%s]\n", inet_ntoa(server_addr.sin_addr));

    if (connect(sockfd, (struct sockaddr*)&server_addr,
                sizeof(struct sockaddr)) == -1) {
        perror("connect");
        exit(1);
    }

    chatting(sockfd);

    close(sockfd);
    return 0;
}

// 채팅 함수
void chatting(int sd)
{
   fd_set mask, read_fds;
   int n, i;
   char ch;
   char buf[MAXDATASIZE];
   char buf1[MAXDATASIZE+50];
   char name[20];
   printf("name : ");
   fgets(name, 20, stdin);
   name[strlen(name)-1]='\0';
   FD_ZERO(&mask);
   FD_ZERO(&read_fds);
   FD_SET(sd, &mask);
   FD_SET(0, &mask);
   while(1) {
       read_fds = mask;
       select(sd+1, &read_fds, NULL, NULL, NULL);
       if(FD_ISSET(sd, &read_fds)) {
           n = recv(sd, buf, MAXDATASIZE-1, 0);
           if(n<=0)
        	   break;
           buf[n] = '\0';
           if(!strncmp(buf, "exit",4))
        	   break;
           printf("%s\n", buf);
       } else if(FD_ISSET(0, &read_fds)) {
           fgets(buf,MAXDATASIZE-1, stdin);
           buf[strlen(buf)-1]='\0';
           sprintf(buf1, "[ %s ] %s", name, buf);
           send(sd, buf1, strlen(buf1), 0);
           if(strncmp(buf, "exit", 4) == 0)
        	   break;
       } else {
    	   printf("Unexpected event\n");
    	   break;
       }
   }
}