#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <pcf8591.h>
#include <pthread.h>
#include "cds_led.h"

#define LED 1
#define CDS_CH 0
#define I2C_ADDR 0x48
#define THRESHOLD 225

// 외부에서 제어할 stop flag
//volatile int cds_running = 1; -> 필요 없음.

void setupCDSLED() {
    wiringPiSetup();
    pinMode(LED, OUTPUT);

    if(pcf8591Setup(I2C_ADDR, I2C_ADDR) == -1) {
        printf("PCF8591 init failed\n");
        exit(1);
    }
}

// 스레드 함수
// void* cdsLedThread(void* arg){
//     int *running = (int*)arg;   // 핵심  flag 전달 받아야 함

//     while(*running){ // 값을 받아야지 정수야!!
//         int cdsValue = analogRead(I2C_ADDR + CDS_CH);
//         printf("CDS Value: %d\n", cdsValue);
//         fflush(stdout);

//         if(cdsValue < THRESHOLD) digitalWrite(LED, LOW);
//         else digitalWrite(LED, HIGH);

//         delay(500);
//     }

//     printf("CDS thread exit\n");
//     // 스레드 종료 시 LED 끄기
//     digitalWrite(LED, LOW);

//     return NULL;
// }
void* cdsLedThread(void* arg){
    int *running = (int*)arg;

    while(running && *running){
        int cdsValue = analogRead(I2C_ADDR + CDS_CH);
        printf("CDS Value: %d\n", cdsValue);
        fflush(stdout);

        if(cdsValue < THRESHOLD) digitalWrite(LED, LOW);
        else digitalWrite(LED, HIGH);

        delay(500);
    }

    printf("CDS thread exit\n");
    digitalWrite(LED, LOW);
    return NULL;
}
