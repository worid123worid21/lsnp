#include <wiringPi.h>
#include <softTone.h>
#include <stdio.h>

#define SPKR 6   /* GPIO25 */
#define SW   5   /* GPIO24 */
#define SW2   4   /* GPIO23 */
//#define TOTAL 46 /* 총 음계 수 */
#define TOTAL 32 /* 총 음계 수 */

// 슈퍼 마리오 멜로디
// int notes[] = {
//   660, 660, 0, 660, 0, 523, 660, 0,
//   784, 0, 0, 0,
//   392, 0, 0, 0,
//   523, 0, 0, 392, 0, 0, 330, 0,
//   0, 440, 0, 494, 0, 466, 440, 0,
//   392, 660, 784, 880, 0,
//   698, 784, 0,
//   660, 0, 523, 587, 494, 0
// };
int notes[] = { /* 학교종을 연주하기 위한 계이름 */ 
    391, 391, 440, 440, 391, 391, 329.63, 329.63,
    391, 391, 329.63, 329.63, 293.66, 293.66, 293.66, 0,
    391, 391, 440, 440, 391, 391, 329.63, 329.63, 
    391, 329.63, 293.66, 329.63, 261.63, 261.63, 261.63, 0
};


void musicPlay() {
    softToneCreate(SPKR);  // 스피커 톤 설정

    for(int i=0; i<TOTAL; i++){
        // SW2 눌리면 음악 즉시 종료
        if(digitalRead(SW2) == LOW){
            softToneWrite(SPKR, 0); // 소리 끄기
            return;
        }

        softToneWrite(SPKR, notes[i]); // 음 출력
        delay(280);                     // 음 길이
    }

    softToneWrite(SPKR, 0); // 음악 끝나면 소리 끄기
}

int main() {
    wiringPiSetup();            // wPi 번호 기준

    pinMode(SW, INPUT);
    pullUpDnControl(SW, PUD_UP); // 풀업

    pinMode(SW2, INPUT);
    pullUpDnControl(SW2, PUD_UP); // 풀업

    while(1){
        // SW 눌리면 음악 재생/재시작
        if(digitalRead(SW) == LOW){
            // 버튼 눌린 상태에서 손 떼기를 기다림
            while(digitalRead(SW) == LOW) delay(10);
            
            musicPlay(); // 음악 재생
        }

        // SW2 누른 상태에서 손 떼기를 기다림 (종료 확인용)
        if(digitalRead(SW2) == LOW){
            while(digitalRead(SW2) == LOW) delay(10);
            softToneWrite(SPKR, 0); // 음악 즉시 종료
        }

        delay(10); // CPU 점유 최소화
    }

    return 0;
}