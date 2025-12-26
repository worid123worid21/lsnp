#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define BUF_SIZE 128

int sock;

// 상태 수신 스레드
void* recvThread(void* arg){
    char buf[BUF_SIZE];
    int n;
    while((n = recv(sock, buf, sizeof(buf)-1, 0)) > 0){
        buf[n] = 0;
        printf("[STATUS] %s", buf);  // 서버 상태 출력
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

    printf("Connected to server %s:5000\n", server_ip);

    // 상태 수신 스레드 시작
    pthread_t tid;
    pthread_create(&tid, NULL, recvThread, NULL);

    // 명령 전송 루프
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
        buf[strcspn(buf,"\n")] = 0;

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

    // 스레드 종료 및 소켓 닫기
    pthread_cancel(tid);
    pthread_join(tid, NULL);
    close(sock);
    printf("Client stopped.\n");
    return 0;
}
