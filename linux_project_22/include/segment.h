#ifndef SEGMENT_H
#define SEGMENT_H

#include <wiringPi.h>
#include <softTone.h>
#include <unistd.h>
#include <stdlib.h>

void setupSegment();
void* segmentThread(void* arg);
void displaySegmentNumber(int num);

#endif
