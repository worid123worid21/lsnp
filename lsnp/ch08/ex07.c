#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h> // 세마포 사용 해더

#define LOOP_MAX 10

int commonCounter= 0;

sem_t semid;   // 전역 세마포어

void * inc_thread(void *);

int main(void)
{
     pthread_t tid1;
     pthread_t tid2;

    // ★ 세마포어 초기화
    // pshared = 0 → 스레드 간 공유
    // value = 1 → 임계영역 1개
    sem_init(&semid, 0, 1);

    // 스레드 생성
     if ((pthread_create( &tid1, NULL, inc_thread, "thread1")) ||
         (pthread_create( &tid2, NULL, inc_thread, "thread2"))) {
          perror("pthread_create");
          exit (errno);
     }

    pthread_join(tid1, (void **)NULL);
    pthread_join(tid2, (void **)NULL);

    // ★ 세마포어 제거
    sem_destroy(&semid);

    return 0;
}

void * inc_thread(void *arg)
{
    int loopCount;
    int temp;
    char buffer[80];
    int i;

    for (loopCount = 0; loopCount < LOOP_MAX; loopCount++) {

        // lock 개념
        sem_wait(&semid);

        sprintf (buffer, "<%s> Common counter :  from %d to ", (char*) arg, commonCounter);
        write(1, buffer, strlen(buffer));

        temp = commonCounter;
        for(i=0; i<900000; i++);        // for delay
        commonCounter = temp + 1;
        sprintf (buffer, "%d\n", commonCounter);
        write(1, buffer, strlen(buffer));

        // unlock 개념
        sem_post(&semid);

        for(i=0; i<500000; i++);        // for delay
    }
}
