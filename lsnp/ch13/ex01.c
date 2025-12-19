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

#define BACKLOG 10
#define MAXDATASIZE 1024 //al

void chatting(int sd);

int main(void)
{
   int sockfd, new_fd;
   struct sockaddr_in server_addr;
   struct sockaddr_in client_addr;
   socklen_t sin_size; //수정
   int yes = 1;

   if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
       perror("socket");
       exit(1);
   }

   //setsocket -> 중복 bind() error 방지
   //SO_REUSEADDR 옵션
   int optval=1;
   setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

   server_addr.sin_family = AF_INET;
   server_addr.sin_port = htons(60000);
   server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
   memset(&(server_addr.sin_zero), '\0', 8);

   if(bind(sockfd, (struct sockaddr *)&server_addr,
		   sizeof(struct sockaddr))==-1) {
       perror("bind");
       exit(1);
   }
   if(listen(sockfd, BACKLOG) == -1) {
       perror("listen");
       exit(1);
   }
   while(1) {
       sin_size = sizeof(struct sockaddr_in);
       if((new_fd = accept(sockfd, (struct sockaddr *) &client_addr,
    		       &sin_size))== -1)  {
           perror("accept");
           continue;
       }
       printf("server : got connection from %s \n",
    		    inet_ntoa(client_addr.sin_addr));
      
       chatting(new_fd);
      
   }
   return 0;
}

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
// 상대방과 대화
// void chatting(int sd)
// {
//     //fdset_t rfdsat;
//     fd_set rfdset;

//     FD_ZERO(&rfdset);
//     FD_SET(0, &rfdset);
//     FD_SET(sd, &rfdset);

//     while(1){
//         //exit 문자열 입력 받으면 나가기!
//         //일대일 채팅이라 가능한 표현 인자
//         select(sd+1, &rfdset, NULL, NULL, NULL);
//         if(FD_ISSET(0, &rfdset)){ // keyboard event
//             //
//         } else if(FD_ISSET(sd, &rfdset)){ //상대방 쪽에서 이벤트 발생
//             //
//         }
//     }
// }