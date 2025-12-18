#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#define LOOP_MAX 10

int commonCounter= 0;

//뮤텍스 선언은 전역으로
pthread_mutex_t mutexid;// 선언

void * inc_thread(void *);

int main(void)
{
     pthread_t tid1;
     pthread_t tid2;

    //mutex는 thread 생성 전에 만들기
    // 뮤텍스 초기화
    pthread_mutex_init(&mutexid, NULL);

    // 스레드 생성
     if ((pthread_create( &tid1, NULL, inc_thread, "thread1")) ||
         (pthread_create( &tid2, NULL, inc_thread, "thread2"))) {
          perror("pthread_create");
          exit (errno);
     }

    //여기? 

    pthread_join(tid1, (void **)NULL);
    pthread_join(tid2, (void **)NULL);

    //여기?
    // 뮤텍스 제거
    pthread_mutex_destroy(&mutexid);

    return 0;
}

void * inc_thread(void *arg)
{
    int loopCount;
    int temp;
    char buffer[80];
    int i;

    // 여기서 실행하면 하나의 쓰레드가 for문이 돌때까지 lock이라서 쓰레드 동작이 아니다.
    for (loopCount = 0; loopCount < LOOP_MAX; loopCount++) {
        //즉,  for문 시작과 끝에서 만들어 준다
        // lock
        pthread_mutex_lock(&mutexid);

        sprintf (buffer, "<%s> Common counter :  from %d to ", (char*) arg, commonCounter);
        write(1, buffer, strlen(buffer));

        temp = commonCounter;
        for(i=0; i<900000; i++);        // for delay
        commonCounter = temp + 1;
        sprintf (buffer, "%d\n", commonCounter);
        write(1, buffer, strlen(buffer));

        // ㅇㅋ for 문 끝
        pthread_mutex_unlock(&mutexid);

        for(i=0; i<500000; i++);        // for delay
        // for 전체의 딜레이라서 이건 상관 없는 거임? -> 맞음
    }
}
