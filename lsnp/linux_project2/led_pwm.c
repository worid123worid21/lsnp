//led_pwm
#include <wiringPi.h>
#include <softPwm.h>
#include <stdio.h>

#define SW   5   /* GPIO24 */
#define LED  1   /* GPIO18 */

/* 밝기 단계 (0~255) */
int brightness[4] = { 0, 80, 160, 255 };

int main(void)
{
    int state = 0;          /* 현재 밝기 상태 */
    int prevSw = HIGH;      /* 이전 버튼 상태 */

    wiringPiSetup();

    pinMode(SW, INPUT);
    pullUpDnControl(SW, PUD_UP);   /* 풀업 */
    pinMode(LED, OUTPUT);

    softPwmCreate(LED, 0, 255);

    while (1) {
        int currSw = digitalRead(SW);

        /* 버튼 눌림 감지 (HIGH → LOW) */
        if (prevSw == HIGH && currSw == LOW) {
            state = (state + 1) % 4;                 /* 상태 변경 */
            softPwmWrite(LED, brightness[state]);    /* 밝기 변경 */
            delay(200);  /* 디바운싱 */
        }

        prevSw = currSw;
        delay(20);
    }

    return 0;
}
