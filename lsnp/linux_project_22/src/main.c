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
#include <sys/stat.h>   // umask

#include "device.h"
#include "cds_led.h"
#include "led_pwm.h"
#include "music.h"
#include "segment.h"

#define PORT 5000
#define BUF_SIZE 128
#define LOG_FILE "/home/suseok/daemon.log"

// 절대 경로로 라이브러리 지정
#define LIB_PATH_CDS "/home/suseok/project/linux_project_2/lib/libcds_led.so"
#define LIB_PATH_PWM "/home/suseok/project/linux_project_2/lib/libled_pwm.so"
#define LIB_PATH_MUSIC "/home/suseok/project/linux_project_2/lib/libmusic.so"
#define LIB_PATH_SEG "/home/suseok/project/linux_project_2/lib/libsegment.so"

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

int main(){
    // ---------------- 데몬화 ----------------
    pid_t pid = fork();
    if(pid < 0) { perror("fork"); exit(1); }
    if(pid > 0) exit(0); // 부모 종료

    if(setsid() < 0) { perror("setsid"); exit(1); }

    // 데몬 로그 파일 출력
    freopen(LOG_FILE, "w", stdout);
    freopen(LOG_FILE, "w", stderr);

    umask(0);
    chdir("/");   // CWD 변경

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

    printf("TCP Daemon Server running on port %d...\n", PORT);

    int client_sock;
    struct sockaddr_in client_addr;
    socklen_t addr_size = sizeof(client_addr);
    char buf[BUF_SIZE];

    client_sock = accept(server_sock,(struct sockaddr*)&client_addr,&addr_size);
    if(client_sock < 0){ perror("accept"); exit(1); }
    printf("Client connected\n");

    while(server_running){
        int n = recv(client_sock,buf,sizeof(buf)-1,0);
        if(n <= 0) break;
        buf[n] = 0;
        printf("Command received: %s\n",buf);

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

        // Exit
        else if(strncmp(buf,"EXIT",4)==0) break;
    }

    stopDevice(&cdsDevice);
    stopDevice(&pwmDevice);
    stopDevice(&musicDevice);
    stopDevice(&segDevice);

    close(client_sock);
    close(server_sock);
    printf("Server stopped.\n");
    return 0;
}

// ---------------- start functions ----------------

void startCDSLED(){
    if(cdsDevice.running){
        printf("CDS LED already running\n");
        return;
    }

    cdsDevice.handle = dlopen(LIB_PATH_CDS, RTLD_LAZY);
    if(!cdsDevice.handle){ printf("dlopen error: %s\n", dlerror()); return; }

    void (*setupCDSLED)() = dlsym(cdsDevice.handle,"setupCDSLED");
    void* (*cdsLedThread)(void*) = dlsym(cdsDevice.handle,"cdsLedThread");
    setupCDSLED();

    cdsDevice.running = malloc(sizeof(int));
    *cdsDevice.running = 1;
    pthread_create(&cdsDevice.thread,NULL,cdsLedThread,cdsDevice.running);
    printf("CDS LED started\n");
}

void startLEDPWM(){
    if(pwmDevice.running){
        printf("LED PWM already running\n");
        return;
    }

    pwmDevice.handle = dlopen(LIB_PATH_PWM, RTLD_LAZY);
    if(!pwmDevice.handle){ printf("dlopen error: %s\n", dlerror()); return; }

    void (*setupLEDPWM)() = dlsym(pwmDevice.handle,"setupLEDPWM");
    void* (*ledPwmThread)(void*) = dlsym(pwmDevice.handle,"ledPwmThread");
    setupLEDPWM();

    pwmDevice.running = malloc(sizeof(int));
    *pwmDevice.running = 1;
    pthread_create(&pwmDevice.thread,NULL,ledPwmThread,pwmDevice.running);
    printf("LED PWM started\n");
}

void startMusic(){
    if(musicDevice.running){
        printf("Music already running\n");
        return;
    }

    musicDevice.handle = dlopen(LIB_PATH_MUSIC, RTLD_LAZY);
    if(!musicDevice.handle){ printf("dlopen error: %s\n", dlerror()); return; }

    void (*setupMusic)() = dlsym(musicDevice.handle,"setupMusic");
    void* (*musicThread)(void*) = dlsym(musicDevice.handle,"musicThread");
    setupMusic();

    musicDevice.running = malloc(sizeof(int));
    *musicDevice.running = 1;
    pthread_create(&musicDevice.thread,NULL,musicThread,musicDevice.running);
    printf("Music started\n");
}

void startSegment(int num){
    if(segDevice.running){
        printf("Segment already running, stopping first\n");
        stopDevice(&segDevice);
    }

    segDevice.handle = dlopen(LIB_PATH_SEG, RTLD_LAZY);
    if(!segDevice.handle){ printf("dlopen error: %s\n", dlerror()); return; }

    void (*setupSegment)() = dlsym(segDevice.handle,"setupSegment");
    void* (*segmentThreadFunc)(void*) = dlsym(segDevice.handle,"segmentThread");
    setupSegment();

    int *startPtr = malloc(sizeof(int));
    *startPtr = num;
    segDevice.running = malloc(sizeof(int));
    *segDevice.running = 1;

    pthread_create(&segDevice.thread,NULL,segmentThreadFunc,startPtr);
}