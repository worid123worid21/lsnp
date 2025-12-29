#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <pcf8591.h>
#include <pthread.h>
#include "cds_led.h"

#define LED 1
#define CDS_CH 0
#define I2C_ADDR 0x48
#define THRESHOLD 235

void setupCDSLED() {
    //wiringPiSetup(); -> 중복 문제 때문에 제거
    pinMode(LED, OUTPUT);

    if(pcf8591Setup(I2C_ADDR, I2C_ADDR) == -1) {
        printf("PCF8591 init failed\n");
        exit(1);
    }
}

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
