#ifndef DEVICE_H
#define DEVICE_H

#include <pthread.h>

// 공통 장치 구조체
typedef struct {
    void* handle;          // dlopen 핸들
    pthread_t thread;      // 스레드 ID
    int *running;          // stop_flag (1: 실행, 0: 종료)
} Device;

// Segment 전용 인자
typedef struct {
    int start_num;         // 시작 숫자
    int *running;          // stop_flag 포인터
} SegmentArg;

#endif
