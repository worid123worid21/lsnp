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

int main(void)
{
   int sockfd, new_fd;
   struct sockaddr_in server_addr;
   struct sockaddr_in client_addr;
   socklen_t c_addr_len;
   int sin_size;
   int yes = 1;

   if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
       perror("socket");
       exit(1);
   }
   server_addr.sin_family = AF_INET;
   server_addr.sin_port = htons(60000);
   server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
   memset(&(server_addr.sin_zero), '\0', 8);

   if(bind(sockfd, (struct sockaddr *)&server_addr,
		   sizeof(struct sockaddr))==-1) {
       perror("bind");
       exit(1);
   }

   while(1) {
        struct sockaddr_in client_addr;
        socklen_t c_addr_len = sizeof(client_addr);
        char buf[100];
        size_t size = 99;
        size = recvfrom(sockfd, buf, size, MSG_NOSIGNAL, (struct sockaddr*)&client_addr, &c_addr_len);
        buf[size] = '\0';
        printf("[ %s ]\n",(char*) inet_ntoa(client_addr.sin_addr));
        printf("recv: %s\n", buf);
   }

   close(sockfd); 
   return 0;
}