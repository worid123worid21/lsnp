#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define BUF_SIZE 128

int sock; // 전역 소켓

// 서버에서 오는 메시지를 계속 받는 스레드
void* recvThread(void* arg){
    char buf[BUF_SIZE];
    while(1){
        int n = recv(sock, buf, sizeof(buf)-1, 0);
        if(n <= 0) break; // 서버 종료
        buf[n] = 0;
        printf("%s", buf); // 서버 로그 출력
        fflush(stdout);
    }
    return NULL;
}

int main(){
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

    printf("Connected to server %s:5000\n\n", server_ip);

    // 서버 로그 수신 스레드 생성
    pthread_t tid;
    pthread_create(&tid, NULL, recvThread, NULL);

    while(1){
        printf("Commands:\n");
        printf("cds       : CDS LED START (현재 값 실시간 표시)\n");
        printf("cs        : CDS LED STOP\n");
        printf("led       : LED PWM START\n");
        printf("ls        : LED PWM STOP\n");
        printf("mus       : MUSIC START\n");
        printf("ms        : MUSIC STOP\n");
        printf("s<num>    : SEGMENT START <num>\n");
        printf("ss        : SEGMENT STOP\n");
        printf("0 or EXIT : Exit client\n> ");
        fflush(stdout);

        if(!fgets(buf,sizeof(buf),stdin)) continue;
        buf[strcspn(buf,"\n")]=0;
        if(strlen(buf)==0) continue;

        if(strcmp(buf,"0")==0 || strcasecmp(buf,"EXIT")==0){
            write(sock,"EXIT",5);
            break;
        }

        if(write(sock, buf, strlen(buf)+1) < 0){
            perror("write");
            break;
        }
    }

    close(sock);
    printf("Client stopped.\n");
    return 0;
}