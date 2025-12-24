// led_pwm.c
#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <pthread.h>
#include "led_pwm.h"

#define SW 5
#define LED 1

int brightness[4] = {0, 80, 160, 255};

void setupLEDPWM() {
    wiringPiSetup();
    pinMode(SW, INPUT);
    pullUpDnControl(SW, PUD_UP);
    pinMode(LED, OUTPUT);
    softPwmCreate(LED, 0, 255);
}

// void* ledPwmThread(void* arg) {
//     // pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
//     // pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
//     int *running = (int*)arg;

//     int state = 0;
//     int prevSw = HIGH;

//     while(*running) {
//         int currSw = digitalRead(SW);

//         if(prevSw == HIGH && currSw == LOW){
//             state = (state + 1) % 4;
//             softPwmWrite(LED, brightness[state]);
//             delay(200);
//         }
//         prevSw = currSw;

//         for(int i=0; i<2 && *running; i++) delay(10);
//         // delay(20);
//         // pthread_testcancel();
//     }

//     printf("LEDPWM thread exit\n");
//     softPwmWrite(LED, 0);
//     return NULL;
// }
void* ledPwmThread(void* arg) {
    int *running = (int*)arg;
    int state = 0;
    int prevSw = HIGH;

    while(running && *running) {
        int currSw = digitalRead(SW);
        if(prevSw == HIGH && currSw == LOW){
            state = (state + 1) % 4;
            softPwmWrite(LED, brightness[state]);
        }
        prevSw = currSw;

        for(int i=0;i<20 && running && *running;i++) delay(10);
    }

    softPwmWrite(LED, 0);   // LED 초기화
    printf("LEDPWM thread exit\n");
    return NULL;
}

