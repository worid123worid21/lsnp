#include <stdio.h>
#include "led.h"
extern pthread_mutex_t mutexid;
extern int state;  // 0: OFF, 1: ON
void *ledControl(void *arg) {
    for (;;) {
        pthread_mutex_lock(&mutexid);
        int s = state;
        pthread_mutex_unlock(&mutexid);
        if (s)
            digitalWrite(LED, HIGH);
        else
            digitalWrite(LED, LOW);
        delay(100); // LED 반응 주기
    }
    return NULL;
}