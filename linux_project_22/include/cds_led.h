#ifndef CDS_LED_H
#define CDS_LED_H

#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <pcf8591.h>
#include <pthread.h>

void setupCDSLED();
void* cdsLedThread(void* arg);

#endif
