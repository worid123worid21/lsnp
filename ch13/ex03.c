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
#include <poll.h>
#include <sys/select.h>
#include <unistd.h> //close

#define BACKLOG 10
void chatting(int sd);

int main(void)
{
   int sockfd, new_fd;
   struct sockaddr_in server_addr;
   struct sockaddr_in client_addr;
   int sin_size;

   if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
       perror("socket");
       exit(1);
   }
   server_addr.sin_family = AF_INET;
   server_addr.sin_port = htons(60000);
   server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
   memset(&(server_addr.sin_zero), '\0', 8);

   if(bind(sockfd, (struct sockaddr *)&server_addr,  sizeof(struct sockaddr))==-1) {
       perror("bind");
       exit(1);
   }
   if(listen(sockfd, BACKLOG) == -1) {
       perror("listen");
       exit(1);
   }
   while(1) {
       sin_size = sizeof(struct sockaddr_in);
       if((new_fd = accept(sockfd, (struct sockaddr *) &client_addr, &sin_size))== -1)  {
           perror("accept");
           continue;
       }
       printf("server : got connection from %s \n", inet_ntoa(client_addr.sin_addr));
       chatting(new_fd);
   }
   return 0;
}

#define MAXDATASIZE	1024

void chatting(int sd)
{
   int n;
   char ch;
   char buf[MAXDATASIZE];
   char buf1[MAXDATASIZE+100];
   char name[20];
   struct pollfd pollfds[2];

   printf("이  름 : ");
   fgets(name, 20, stdin);
   name[strlen(name)-1]='\0';

   pollfds[0].fd=0;
   pollfds[0].events=POLLIN;
   pollfds[1].fd=sd;
   pollfds[1].events=POLLIN;
   while(1) {
	   if(poll(pollfds, 2, -1)==-1) {
		   perror("poll");
		   return;
	   }
       if(pollfds[1].revents & POLLIN) {
           n = recv(sd, buf, MAXDATASIZE-1, 0);
           if(n<=0)
        	   break;

           buf[n] = '\0';
           if(!strcmp(buf, "..."))
        	   break;
           printf("%s\n", buf);
       } else if(pollfds[0].revents & POLLIN) {
           fgets(buf,MAXDATASIZE-1, stdin);
           buf[strlen(buf)-1]='\0';
           sprintf(buf1, "[ %s ] %s", name, buf);
           send(sd, buf1, strlen(buf1), 0);
           if(strncmp(buf, "...", 3) == 0)
        	   break;
       } else {
    	   printf("Unexpected event\n");
    	   break;
       }
   }
}
//
