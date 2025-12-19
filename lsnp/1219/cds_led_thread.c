// main.c
#include <wiringPi.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

pthread_mutex_t mutexid;

#define SW   5   /* GPIO24 */
#define CDS  0   /* GPIO17 */
#define LED  1   /* GPIO18 */

int flag = 0;

/* 🔹 공유 라이브러리 함수 선언만 추가 */
void* ledControl(void* arg);
void* cdsControl(void* arg);

int main(void)
{
    pthread_t led_thread, sensor_thread;

    pthread_mutex_init(&mutexid, NULL);

    wiringPiSetup();
    pinMode(SW, INPUT);
    pinMode(CDS, INPUT);
    pinMode(LED, OUTPUT);

    /* 🔹 기존 흐름 그대로 */
    pthread_create(&led_thread, NULL, ledControl, NULL);
    pthread_create(&sensor_thread, NULL, cdsControl, NULL);

    pthread_join(sensor_thread, NULL);
    pthread_join(led_thread, NULL);

    pthread_mutex_destroy(&mutexid);
    return 0;
}
