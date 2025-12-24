#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>

#define SERVER_PORT 5000
#define BUF_SIZE 128

volatile sig_atomic_t client_running = 1;

// SIGTERM 처리: 안전 종료
void handle_sigterm(int sig){
    client_running = 0;
    printf("\nClient stopping...\n");
}

int main(){
    signal(SIGTERM, handle_sigterm);  // SIGTERM 수신 시 안전 종료
    signal(SIGINT, SIG_IGN);          // Ctrl+C 무시

    char server_ip[32];
    printf("Enter server IP: ");
    if(!fgets(server_ip, sizeof(server_ip), stdin)) exit(1);
    server_ip[strcspn(server_ip, "\n")] = 0;  // 개행 제거

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0){ perror("socket"); exit(1); }

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);
    if(inet_pton(AF_INET, server_ip, &serv_addr.sin_addr) <= 0){
        perror("inet_pton"); exit(1);
    }

    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0){
        perror("connect"); exit(1);
    }

    printf("Connected to server %s:%d\n", server_ip, SERVER_PORT);

    char buf[BUF_SIZE];
    while(client_running){
        printf("\nCommands:\n");
        printf("CDSLED START / CDSLED STOP\n");
        printf("LEDPWM START / LEDPWM STOP\n");
        printf("MUSIC START / MUSIC STOP\n");
        printf("SEGMENT START <num> / SEGMENT STOP\n");
        printf("0 or EXIT to quit\n> ");
        fflush(stdout);

        if(!fgets(buf, sizeof(buf), stdin)) continue;

        // 종료 명령
        if(buf[0] == '0' || strcasecmp(buf, "EXIT\n") == 0){
            send(sock, "EXIT\n", 5, 0);
            break;
        }

        // fgets로 읽은 문자열 끝에 개행 추가 보장
        if(buf[strlen(buf)-1] != '\n'){
            buf[strlen(buf)] = '\n';
            buf[strlen(buf)+1] = 0;
        }

        // 서버로 전송
        if(send(sock, buf, strlen(buf), 0) < 0){
            perror("send");
            break;
        }
    }

    close(sock);
    printf("Client stopped.\n");
    return 0;
}
