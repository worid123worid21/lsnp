#include <wiringPi.h>
#include <softTone.h>
#include <stdio.h>
#include <stdlib.h>

/* FND 핀 (wPi 기준) */
#define A 23
#define B 22
#define C 27
#define D 28
#define E 29
#define F 24
#define G 25

#define BUZZER 6   // GPIO24

/* 도~도 음계 (Hz) */
#define DO   262
#define RE   294
#define MI   330
#define FA   349
#define SOL  392
#define LA   440
#define SI   494
#define DO2  523

int scale[] = {DO, RE, MI, FA, SOL, LA, SI, DO2};

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

void fndDisplay(int num){
    for(int i=0;i<7;i++){
        pinMode(fnd_pins[i], OUTPUT);
        digitalWrite(fnd_pins[i], num_pattern[num][i]?LOW:HIGH);
    }
}

/* 도레미파솔라시도 출력 */
void playScale(){
    softToneCreate(BUZZER);

    for(int i=0;i<8;i++){
        softToneWrite(BUZZER, scale[i]);
        delay(300);
    }
    softToneWrite(BUZZER, 0); // 소리 끄기
}

int main(void){
    wiringPiSetup();
    char input[3];
    int start_num;

    while(1){
        /* 키보드 입력 */
        do{
            printf("1~9 숫자를 입력하세요: ");
            fgets(input, sizeof(input), stdin);
            start_num = atoi(input);
        } while(start_num < 1 || start_num > 9);

        int count = start_num;

        /* 카운트다운 */
        while(count >= 0){
            fndDisplay(count);
            delay(1000);
            count--;

            if(count <= 0){
                fndDisplay(0);
                playScale();  // 도레미파솔라시도
            }
        }
    }
}
