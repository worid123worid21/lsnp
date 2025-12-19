// cdsControl.c
#include <wiringPi.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define CDS 0   // GPIO17 (wiringPi 번호)

extern int flag;                 // main.c 에서 정의됨
extern pthread_mutex_t mutexid;  // main.c 에서 정의됨

void* cdsControl(void* arg)
{
    while (1) {
        if (digitalRead(CDS) == HIGH) {
            pthread_mutex_lock(&mutexid);
            flag = 1;
            pthread_mutex_unlock(&mutexid);
        } else {
            pthread_mutex_lock(&mutexid);
            flag = 0;
            pthread_mutex_unlock(&mutexid);
        }
        delay(50);
    }
    return NULL;
}
