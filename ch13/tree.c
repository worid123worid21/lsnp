#include <wiringPi.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
pthread_mutex_t mutexid;
#define SW  5   /* GPIO24 */
#define CDS     0   /* GPIO17 */
#define LED     1   /* GPIO18 */
int flag = 0;
void* cdsControl(void * any)
{
    for (;;) {          /* 조도 센서 검사를 위해 무한 루프를 실행한다. */
        if(digitalRead(CDS) == HIGH) {  /* 빛이 감지되면(HIGH) */
            pthread_mutex_lock(&mutexid);
            flag  = 1;
            pthread_mutex_unlock(&mutexid);
            delay(50);
        }
        else {
            pthread_mutex_lock(&mutexid);
            flag = 0;
            pthread_mutex_unlock(&mutexid);
            delay(50);
        }
    }
    return 0;
}
void* ledControl(void * any){
    while(1){
    pthread_mutex_lock(&mutexid);
    if(flag) digitalWrite(LED, HIGH);   /* LED 켜기(On) */
    else digitalWrite(LED, LOW);
    pthread_mutex_unlock(&mutexid);
    delay(50);
    }
}
int main( )
{
    pthread_t led_thread, sensor_thread;
    pthread_mutex_init(&mutexid, NULL);
    wiringPiSetup();
    pinMode(SW, INPUT);
    pinMode(CDS, INPUT);
    pinMode(LED, OUTPUT);
    pthread_create(&led_thread,NULL,ledControl,(void*)NULL);
    pthread_create(&sensor_thread,NULL,cdsControl,(void*)NULL);
    pthread_join(sensor_thread, NULL);
    pthread_join(led_thread, NULL);
    return 0;
}