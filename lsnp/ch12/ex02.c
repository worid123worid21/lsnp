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

#define MAXDATASIZE 100 

int main(int argc, char*argv[]) { 
    int sockfd, numbytes; 
    socklen_t addr_len; 
    char buf[MAXDATASIZE] = "Hello Monkey!!"; 
    struct hostent *he; 
    struct sockaddr_in server_addr; 
    
    if(argc!= 2) { 
        fprintf(stderr, "usage : client hostname \n"); 
        exit(1); 
    } 
    
    if((he = gethostbyname(argv[1])) == NULL) { 
        perror("gethostbyname"); 
        exit(1); 
    } 
    
    if((sockfd= socket(AF_INET, SOCK_DGRAM, 0)) == -1) { 
        perror("socket"); 
        exit(1); 
    } 
    
    server_addr.sin_family= AF_INET; 
    server_addr.sin_port= htons(60000); 
    server_addr.sin_addr= *((struct in_addr*)he->h_addr_list[0]); 
    printf("[ %s ]\n",(char*) inet_ntoa(server_addr.sin_addr));
    memset(&(server_addr.sin_zero), '\0',8); 
    
    // if((numbytes= sendto(sockfd, buf, MAXDATASIZE-1, 0)) == -1) { 
    //     perror("recv"); 
    //     exit(1); 
    // } 

    sendto(sockfd, buf, strlen(buf), MSG_NOSIGNAL, (struct sockaddr*)&server_addr, sizeof(server_addr));
    
    // buf[numbytes] = '\0'; 
    // printf("Received : %s\n", buf); 
    close(sockfd); 
    return 0; 
}