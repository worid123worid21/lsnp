#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main(void)
{
    struct hostent*host;
    struct sockaddr_inserver_addr;

    if((host = gethostbyname("ubuntu.linux.com")) == NULL)
    {
        printf("error\n");
        exit(1);
    }

    server_addr.sin_family= AF_INET;
    server_addr.sin_port= htons(0x0050);
    server_addr.sin_addr= *((structin_addr*) host->h_addr);

    memset(&(server_addr.sin_zero), '\0',8);
    printf("[ %s ]\n",  inet_ntoa(server_addr.sin_addr));
    return 0;
}