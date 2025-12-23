#include <stdio.h>
#include "led.h"
extern pthread_mutex_t mutexid;
extern int state;  // 0: OFF, 1: ON
void *cdsControl(void *arg) {
    for (;;) {
        int val = digitalRead(CDS);
        pthread_mutex_lock(&mutexid);
        state = (val == HIGH) ? 1 : 0;
        pthread_mutex_unlock(&mutexid);
        delay(100); // 센서 읽기 주기
    }
    return NULL;
}