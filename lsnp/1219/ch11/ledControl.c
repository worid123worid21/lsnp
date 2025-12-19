// ledControl.c
#include <wiringPi.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define LED 1   // GPIO18 (wiringPi 번호)

extern int flag;                 // main.c 에서 정의됨
extern pthread_mutex_t mutexid;  // main.c 에서 정의됨

void* ledControl(void* arg)
{
    while (1) {
        pthread_mutex_lock(&mutexid);
        if (flag)
            digitalWrite(LED, HIGH);
        else
            digitalWrite(LED, LOW);
        pthread_mutex_unlock(&mutexid);

        delay(50);
    }
    return NULL;
}
