#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <sys/ioctl.h>

int main(int argc, char **argv)
{
    struct sockaddr_in serveraddr, clientaddr;
    int sockfd;
    int client_sd[100];
    int idx = 0;
    int  socklen;
    int n, i;
    int readn;
    struct epoll_event ev,*client_list;
    int efd;
    char buf_in[256];

    // fd 배열 초기화
    for(int i = 0; i < 100; i++) { client_sd[i] = -1; }

    client_list = (struct epoll_event *)malloc(sizeof(*client_list) * 20);
    if ((efd = epoll_create(100)) < 0) {
            perror("epoll_create");
            exit(1);
    }
    socklen = sizeof(clientaddr);
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0))==-1) {
        perror("socket error :");
        close(sockfd);
        return 1;
    }
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(60000);
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind (sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1) {
        close(sockfd);
        return 1;
    }
    listen(sockfd, 5);
    ev.events = EPOLLIN;
    ev.data.fd = sockfd;
    epoll_ctl(efd, EPOLL_CTL_ADD, sockfd, &ev);
    while(1) {
        if((n = epoll_wait(efd, client_list, 20, -1))==-1) {
            perror("epoll_wait");
        }
        for (i = 0;i < n; i++){
            if (client_list[i].data.fd == sockfd) {
                printf("Accept\n");
                int temp_fd;
                temp_fd = accept(sockfd, (struct sockaddr *)&clientaddr, & socklen);

                // fd 배열 세팅
                for (int j = 0; j < 100; j++) {
                    if (client_sd[j] == -1) {
                        client_sd[j] = temp_fd;
                        break;
                    }
                }

                ev.events = EPOLLIN;
                ev.data.fd = temp_fd;
                epoll_ctl(efd, EPOLL_CTL_ADD, temp_fd, &ev);
                idx++;
            } else {
                readn = read(client_list[i].data.fd, buf_in, 255);
                if(readn <= 0){
                    epoll_ctl(efd, EPOLL_CTL_DEL, client_list[i].data.fd, client_list);
                    close(client_list[i].data.fd);
                    printf("Close fd\n");

                    // fd 배열에서 삭제
                    for (int j = 0; j < 100; j++) {
                        if (client_sd[j] == client_list[j].data.fd) {
                            client_sd[j] = -1;
                            idx--;
                            break;
                        }
                    }
                } else {
                    buf_in[readn] = '\0';
                    printf("read data %s\n", buf_in);
                    printf("read fd: %d\n", client_list[i].data.fd);

                    int cnt = 0;
                    for (int j = 0; j < 100; j++) {
                        if (cnt == idx) {break;}
                        if (client_sd[j] == client_list[i].data.fd) { continue; }
                        if (client_sd[j] != -1) {
                            send(client_sd[j], buf_in, readn, 0);
                            cnt++;
                        }
                    }
                }
            }   // else-end
         } // for-end
    } // while -end
    return 0;
}