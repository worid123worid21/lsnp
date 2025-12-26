#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <dlfcn.h>
#include <arpa/inet.h>
#include <wiringPi.h>
#include <ctype.h>
#include <sys/stat.h>

#include "device.h"
#include "cds_led.h"
#include "led_pwm.h"
#include "music.h"
#include "segment.h"

#define PORT 5000
#define BUF_SIZE 128

volatile sig_atomic_t server_running = 1;

void handle_sigint(int sig){
    server_running = 0;
    printf("\nServer stopping...\n");
}

// 장치별 전역 구조체
Device cdsDevice = {0}, pwmDevice = {0}, musicDevice = {0}, segDevice = {0};

void stopDevice(Device *dev){
    if(dev->running){
        *dev->running = 0;
        pthread_join(dev->thread,NULL);
        free(dev->running);
        dev->running = NULL;

        if(dev->handle) dlclose(dev->handle);
        dev->handle = NULL;

        printf("Device stopped\n");
    }
}

// start 함수 선언
void startCDSLED();
void startLEDPWM();
void startMusic();
void startSegment(int num);

// ------------------- 데몬화 함수 -------------------
void daemonize(){
    pid_t pid;

    pid = fork();
    if(pid < 0) exit(EXIT_FAILURE);
    if(pid > 0) exit(EXIT_SUCCESS); // 부모 종료

    if(setsid() < 0) exit(EXIT_FAILURE);

    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    pid = fork();
    if(pid < 0) exit(EXIT_FAILURE);
    if(pid > 0) exit(EXIT_SUCCESS); // 1차 자식 종료

    chdir("/");
    umask(0);

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
}

// ------------------- main -------------------
int main(int argc, char *argv[]){
    // 명령행 인자로 "-d" 주면 데몬 실행
    if(argc > 1 && strcmp(argv[1], "-d") == 0){
        daemonize();
    }

    wiringPiSetup();
    signal(SIGINT, handle_sigint);

    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(server_sock < 0){ perror("socket"); exit(1); }

    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if(bind(server_sock,(struct sockaddr*)&server_addr,sizeof(server_addr))<0){ perror("bind"); exit(1); }
    if(listen(server_sock,5)<0){ perror("listen"); exit(1); }

    if(argc <= 1 || strcmp(argv[1], "-d") != 0){
        printf("TCP Server running on port %d...\n", PORT);
    }

    int client_sock;
    struct sockaddr_in client_addr;
    socklen_t addr_size = sizeof(client_addr);
    char buf[BUF_SIZE];

    while(server_running){
        client_sock = accept(server_sock,(struct sockaddr*)&client_addr,&addr_size);
        if(client_sock < 0) continue;

        while(server_running){
            int n = recv(client_sock,buf,sizeof(buf)-1,0);
            if(n <= 0) break;
            buf[n] = 0;

            // CDS LED
            if(strncmp(buf,"cds",3)==0) startCDSLED();
            else if(strncmp(buf,"cs",2)==0) stopDevice(&cdsDevice);

            // LED PWM
            else if(strncmp(buf,"led",3)==0) startLEDPWM();
            else if(strncmp(buf,"ls",2)==0) stopDevice(&pwmDevice);

            // Music
            else if(strncmp(buf,"mus",3)==0) startMusic();
            else if(strncmp(buf,"ms",2)==0) stopDevice(&musicDevice);

            // Segment
            else if(strncmp(buf,"s",1)==0 && isdigit(buf[1])){
                int num = atoi(buf+1);
                startSegment(num);
            }
            else if(strncmp(buf,"ss",2)==0) stopDevice(&segDevice);

            else if(strncmp(buf,"EXIT",4)==0) break;
        }
        close(client_sock);
    }

    stopDevice(&cdsDevice);
    stopDevice(&pwmDevice);
    stopDevice(&musicDevice);
    stopDevice(&segDevice);

    close(server_sock);
    return 0;
}

// ---------------- start functions ----------------

void startCDSLED(){
    if(cdsDevice.running) return;

    cdsDevice.handle = dlopen("./lib/libcds_led.so", RTLD_LAZY);
    if(!cdsDevice.handle) return;

    void (*setupCDSLED)() = dlsym(cdsDevice.handle,"setupCDSLED");
    void* (*cdsLedThread)(void*) = dlsym(cdsDevice.handle,"cdsLedThread");
    setupCDSLED();

    cdsDevice.running = malloc(sizeof(int));
    *cdsDevice.running = 1;
    pthread_create(&cdsDevice.thread,NULL,cdsLedThread,cdsDevice.running);
}

void startLEDPWM(){
    if(pwmDevice.running) return;

    pwmDevice.handle = dlopen("./lib/libled_pwm.so", RTLD_LAZY);
    if(!pwmDevice.handle) return;

    void (*setupLEDPWM)() = dlsym(pwmDevice.handle,"setupLEDPWM");
    void* (*ledPwmThread)(void*) = dlsym(pwmDevice.handle,"ledPwmThread");
    setupLEDPWM();

    pwmDevice.running = malloc(sizeof(int));
    *pwmDevice.running = 1;
    pthread_create(&pwmDevice.thread,NULL,ledPwmThread,pwmDevice.running);
}

void startMusic(){
    if(musicDevice.running) return;

    musicDevice.handle = dlopen("./lib/libmusic.so", RTLD_LAZY);
    if(!musicDevice.handle) return;

    void (*setupMusic)() = dlsym(musicDevice.handle,"setupMusic");
    void* (*musicThread)(void*) = dlsym(musicDevice.handle,"musicThread");
    setupMusic();

    musicDevice.running = malloc(sizeof(int));
    *musicDevice.running = 1;
    pthread_create(&musicDevice.thread,NULL,musicThread,musicDevice.running);
}

void startSegment(int num){
    if(segDevice.running){
        stopDevice(&segDevice);
    }

    segDevice.handle = dlopen("./lib/libsegment.so", RTLD_LAZY);
    if(!segDevice.handle) return;

    void (*setupSegment)() = dlsym(segDevice.handle,"setupSegment");
    void* (*segmentThreadFunc)(void*) = dlsym(segDevice.handle,"segmentThread");
    setupSegment();

    int *startPtr = malloc(sizeof(int));
    *startPtr = num;
    segDevice.running = malloc(sizeof(int));
    *segDevice.running = 1;

    pthread_create(&segDevice.thread,NULL,segmentThreadFunc,startPtr);
}