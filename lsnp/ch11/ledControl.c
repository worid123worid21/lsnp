#ifdef __arm__
#include <wiringPi.h>
#else
#define HIGH 1
#define LOW 0
static inline int digitalRead(int pin) { return 0; }
static inline void digitalWrite(int pin, int value) {}
static inline void delay(int ms) {}
#endif

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define LED 1

extern int flag;
extern pthread_mutex_t mutexid;

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
