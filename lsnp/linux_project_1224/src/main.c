#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <dlfcn.h>
#include <signal.h>
#include <arpa/inet.h>

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
        if(dev->handle) dlclose(dev->handle);
        dev->handle = NULL;
        dev->running = NULL;
        printf("Device stopped\n");
    }
}

void startCDSLED(){
    if(cdsDevice.running){
        printf("CDS LED already running\n");
        return;
    }

    cdsDevice.handle = dlopen("./lib/libcds_led.so", RTLD_LAZY);
    if(!cdsDevice.handle){
        printf("dlopen error: %s\n", dlerror());
        return;
    }

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

    pwmDevice.handle = dlopen("./lib/libled_pwm.so", RTLD_LAZY);
    if(!pwmDevice.handle){
        printf("dlopen error: %s\n", dlerror());
        return;
    }

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

    musicDevice.handle = dlopen("./lib/libmusic.so", RTLD_LAZY);
    if(!musicDevice.handle){
        printf("dlopen error: %s\n", dlerror());
        return;
    }

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
        printf("Segment already running\n");
        return;
    }

    segDevice.handle = dlopen("./lib/libsegment.so", RTLD_LAZY);
    if(!segDevice.handle){
        printf("dlopen error: %s\n", dlerror());
        return;
    }

    void (*setupSegment)() = dlsym(segDevice.handle,"setupSegment");
    void* (*segmentThread)(void*) = dlsym(segDevice.handle,"segmentThread");
    setupSegment();

    SegmentArg *arg = malloc(sizeof(SegmentArg));
    arg->start_num = num;
    arg->running = malloc(sizeof(int));
    *(arg->running) = 1;

    segDevice.running = arg->running;
    pthread_create(&segDevice.thread,NULL,segmentThread,arg);
    printf("Segment started from %d\n", num);
}

int main(){
    signal(SIGINT, handle_sigint);

    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(server_sock < 0){ perror("socket"); exit(1); }

    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if(bind(server_sock,(struct sockaddr*)&server_addr,sizeof(server_addr))<0){ perror("bind"); exit(1); }
    if(listen(server_sock,5)<0){ perror("listen"); exit(1); }

    printf("TCP Server running on port %d...\n", PORT);

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

        if(strncmp(buf,"cds",3)==0) startCDSLED();
        else if(strncmp(buf,"cs",2)==0) stopDevice(&cdsDevice);
        else if(strncmp(buf,"led",3)==0) startLEDPWM();
        else if(strncmp(buf,"ls",2)==0) stopDevice(&pwmDevice);
        else if(strncmp(buf,"mus",3)==0) startMusic();
        else if(strncmp(buf,"ms",2)==0) stopDevice(&musicDevice);
        else if(strncmp(buf,"s",1)==0){
            int num = atoi(buf+1);
            startSegment(num);
        }
        else if(strncmp(buf,"ss",2)==0) stopDevice(&segDevice);
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
