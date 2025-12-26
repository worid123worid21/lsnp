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
    // wiringPiSetup()는 main에서 이미 호출됨
    pinMode(SW, INPUT);
    pinMode(SW2, INPUT);
    pullUpDnControl(SW, PUD_UP);
    pullUpDnControl(SW2, PUD_UP);
}

// musicPlay 함수에 running flag 체크 추가
void musicPlay(int *running) {
    softToneCreate(SPKR);
    for(int i = 0; i < TOTAL && *running; i++){
        if(digitalRead(SW2) == LOW){
            softToneWrite(SPKR, 0);
            return;
        }
        softToneWrite(SPKR, notes[i]);
        int delay_ms = 280;
        // delay를 작은 단위로 쪼개 running 체크
        for(int j = 0; j < delay_ms / 10 && *running; j++) delay(10);
    }
    softToneWrite(SPKR, 0);
}

void* musicThread(void* arg) {
    int *running = (int*)arg;

    softToneCreate(SPKR);

    while(running && *running){
        // SW 버튼 눌림 확인
        if(digitalRead(SW) == LOW){
            while(digitalRead(SW) == LOW && *running) delay(10);
            if(*running) musicPlay(running);
        }

        // SW2 버튼 눌림 시 즉시 멈춤
        if(digitalRead(SW2) == LOW){
            while(digitalRead(SW2) == LOW && *running) delay(10);
            softToneWrite(SPKR, 0);
        }

        // 스레드 종료 체크
        delay(10);
    }

    softToneWrite(SPKR, 0);
    printf("Music thread exit\n");
    return NULL;
}
