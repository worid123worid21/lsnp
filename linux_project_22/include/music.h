#ifndef MUSIC_H
#define MUSIC_H

#include <stdio.h>
#include <wiringPi.h>
#include <softTone.h>
#include <pthread.h>

void setupMusic();
void* musicThread(void* arg);

#endif
