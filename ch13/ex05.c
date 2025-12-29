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
    int client_sd;
    int  socklen;
    int n, i;
    int readn;
    struct epoll_event ev,*client_list;
    int efd;
    char buf_in[256];
    int count=0; // 추가
    int c_fds[20]; // 추가

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
                //배열로 저장하고 있기, 다중 채팅을 위해
                printf("Accept\n");
                client_sd = accept(sockfd, (struct sockaddr *)&clientaddr, & socklen);

                ev.events = EPOLLIN;
                ev.data.fd = client_sd;
                epoll_ctl(efd, EPOLL_CTL_ADD, client_sd, &ev);
            } else {
                //추가
                memset(buf_in, 0, sizeof(buf_in)); // <-- 수정 1: 버퍼 초기화
                //추가

                readn = read(client_list[i].data.fd, buf_in, 255);
                if(readn <= 0){
                    epoll_ctl(efd, EPOLL_CTL_DEL, client_list[i].data.fd, client_list); // 연결이 끊어진 경우(반환 값이 0?인 경우), 지우기
                    close(client_list[i].data.fd);
                    printf("Close fd\n");
                } else {
                    //추가
                    buf_in[readn] = '\0'; // <-- 수정 2: 문자열 안전 처리
                    //추가

                    printf("read data %s\n", buf_in);
                    //write(client_list[i].data.fd, buf_in, readn); // 이 부분 수정으로 다중 채팅 가능.

                    // <-- 수정 3: 다중 채팅용 브로드캐스트
                    for(int j=0; j<n; j++){
                        int dest_fd = client_list[j].data.fd;

                        if(dest_fd != sockfd && dest_fd != client_list[i].data.fd){
                            write(dest_fd, buf_in, readn);
                        }
                    }
                    //추가
                }
            }   // else-end
         } // for-end
    } // while -end
    return 0;
}