// music.c
#include <stdio.h>
#include <wiringPi.h>
#include <softTone.h>
#include <pthread.h>
#include "music.h"

#define SPKR 6
#define SW 5
#define SW2 4

int notes[] = {
    391, 391, 440, 440, 391, 391, 329, 329,
    391, 391, 329, 329, 293, 293, 293, 0,
    391, 391, 440, 440, 391, 391, 329, 329,
    391, 329, 293, 329, 261, 261, 261, 0
};

#define TOTAL 32

void setupMusic() {
    wiringPiSetup();
    pinMode(SW, INPUT);
    pinMode(SW2, INPUT);
    pullUpDnControl(SW, PUD_UP);
    pullUpDnControl(SW2, PUD_UP);
}

void musicPlay() {
    softToneCreate(SPKR);
    for(int i=0;i<TOTAL;i++){
        if(digitalRead(SW2) == LOW){
            softToneWrite(SPKR, 0);
            return;
        }
        softToneWrite(SPKR, notes[i]);
        delay(280);
    }
    softToneWrite(SPKR, 0);
}

void* musicThread(void* arg) {
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

    softToneCreate(SPKR);

    while(1){
        if(digitalRead(SW) == LOW){
            while(digitalRead(SW) == LOW) delay(10);
            musicPlay();
        }
        if(digitalRead(SW2) == LOW){
            while(digitalRead(SW2) == LOW) delay(10);
            softToneWrite(SPKR, 0);
        }
        delay(10);
        pthread_testcancel();
    }

    softToneWrite(SPKR, 0);
    return NULL;
}