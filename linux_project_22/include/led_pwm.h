#ifndef LED_PWM_H
#define LED_PWM_H

#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <pthread.h>

void setupLEDPWM();
void* ledPwmThread(void* arg);

#endif
