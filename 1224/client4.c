#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUF_SIZE 128

int main(){
    int sock;
    struct sockaddr_in serv_addr;
    char buf[BUF_SIZE];

    char server_ip[32];
    printf("Enter server IP: ");
    if(!fgets(server_ip,sizeof(server_ip),stdin)) exit(1);
    server_ip[strcspn(server_ip,"\n")] = 0;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0){ perror("socket"); exit(1); }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000);

    if(inet_pton(AF_INET, server_ip, &serv_addr.sin_addr) <= 0){
        perror("inet_pton"); exit(1);
    }

    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0){
        perror("connect"); exit(1);
    }

    printf("Connected to server %s:5000\n", server_ip);

    while(1){
        printf("\nCommands:\n");
        printf("cds START / CDSLED STOP\n");
        printf("실사용 cds / cs\n");
        printf("LEDPWM START / LEDPWM STOP\n");
        printf("실사용 led / ls\n");
        printf("MUSIC START / MUSIC STOP\n");
        printf("실사용 mus / ms\n");
        printf("SEGMENT START <num> / SEGMENT STOP\n");
        printf("실사용 s <num> / ss\n");
        printf("EXIT or 0 to quit\n> ");
        fflush(stdout);

        if(!fgets(buf,sizeof(buf),stdin)) continue;
        buf[strcspn(buf,"\n")] = 0; // 개행 제거

        if(strlen(buf)==0) continue;

        if(strcmp(buf,"0")==0 || strcasecmp(buf,"EXIT")==0){
            write(sock,"EXIT",4);
            break;
        }

        int n = write(sock, buf, strlen(buf));
        if(n < 0){
            perror("write");
            break;
        }
    }

    close(sock);
    printf("Client stopped.\n");
    return 0;
}
