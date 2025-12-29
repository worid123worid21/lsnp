// cdsControl.c
#ifdef __arm__
#include <wiringPi.h>
#else
// Ubuntu(x86)용 mock 정의
#define HIGH 1
#define LOW 0
static inline int digitalRead(int pin) { return 0; }
static inline void digitalWrite(int pin, int value) {}
static inline void delay(int ms) {}
#endif

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define CDS 0

extern int flag;
extern pthread_mutex_t mutexid;

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
