#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <dlfcn.h>
#define SW 5
#define CDS 0
#define LED 1
extern int state;  // 0: OFF, 1: ON
extern pthread_mutex_t mutexid;
void *cdsControl(void *arg);
void *ledControl(void *arg);
#endif //_LED_H_