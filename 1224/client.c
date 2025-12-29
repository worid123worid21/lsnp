#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>

#define SERVER_IP "192.168.0.35"  // 라즈베리파이 IP로 변경 가능
#define SERVER_PORT 5000

volatile sig_atomic_t client_running = 1;

void handle_sigterm(int sig){
    client_running = 0;
    printf("\nClient stopping...\n");
}

int main(){
    //signal(SIGINT, SIG_IGN);     // Ctrl+C 무시
    signal(SIGTERM, handle_sigterm);

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0){ perror("socket"); exit(1); }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    if(connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        perror("connect");
        exit(1);
    }

    printf("Connected to server %s:%d\n", SERVER_IP, SERVER_PORT);

    char buf[128];
    while(client_running){
        printf("\nEnter command:\n");
        printf("1. CDSLED START\n2. LEDPWM START\n3. MUSIC START\n4. SEGMENT START <num>\n0. EXIT\n> ");
        fflush(stdout);

        if(!fgets(buf,sizeof(buf),stdin)) continue;

        if(buf[0]=='0'){  // 종료
            send(sock,"EXIT\n",5,0);
            break;
        }

        send(sock, buf, strlen(buf), 0);
    }

    close(sock);
    printf("Client stopped.\n");
    return 0;
}
