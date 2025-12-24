// segment.c
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <softTone.h>
#include <pthread.h>
#include "segment.h"

#define A 23
#define B 22
#define C 27
#define D 28
#define E 29
#define F 24
#define G 25
#define BUZZER 6

int scale[] = {262,294,330,349,392,440,494,523};
int num_pattern[10][7] = {
    {1,1,1,1,1,1,0},
    {0,1,1,0,0,0,0},
    {1,1,0,1,1,0,1},
    {1,1,1,1,0,0,1},
    {0,1,1,0,0,1,1},
    {1,0,1,1,0,1,1},
    {1,0,1,1,1,1,1},
    {1,1,1,0,0,0,0},
    {1,1,1,1,1,1,1},
    {1,1,1,1,0,1,1}
};
int fnd_pins[7] = {A,B,C,D,E,F,G};

void setupSegment(){
    wiringPiSetup();
    for(int i=0;i<7;i++) pinMode(fnd_pins[i], OUTPUT);
}

void fndDisplay(int num){
    for(int i=0;i<7;i++)
        digitalWrite(fnd_pins[i], num_pattern[num][i]?LOW:HIGH);
}

void playScale(){
    softToneCreate(BUZZER);
    for(int i=0;i<8;i++){
        softToneWrite(BUZZER, scale[i]);
        delay(300);
    }
    softToneWrite(BUZZER, 0);
}

void* segmentThread(void* arg){
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

    int start = *(int*)arg;
    for(int count=start; count>=0; count--){
        fndDisplay(count);
        delay(1000);
        pthread_testcancel();
    }
    playScale();
    return NULL;
}