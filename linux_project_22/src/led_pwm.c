#include "led_pwm.h"

#define SW 5
#define LED 1

int brightness[4] = {0, 80, 160, 255};

void setupLEDPWM() {
    //wiringPiSetup();
    pinMode(SW, INPUT);
    pullUpDnControl(SW, PUD_UP);
    pinMode(LED, OUTPUT);
    softPwmCreate(LED, 0, 255);
}

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
