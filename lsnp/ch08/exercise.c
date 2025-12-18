//  exercise.c는 뮤텍스를 이용하여 쓰레드간의 동기화를 구현한 예제 프로그램이다. thread_function쓰
//  레드에서 수행 도중 비정상적으로 종료되는 경우, 뮤텍스가 잠긴 채 종료되면 다른 쓰레드에서 문제가 될 
//  수 있으므로 클린업 처리를 하려고 한다. 소스코드를 완성하시오.

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <stdlib.h>

#define NUMTHREADS 3

pthread_mutex_tmutexid= PTHREAD_MUTEX_INITIALIZER;
int count=0;

//여기에서 해결 cleanup
void cleanup_handler(void* arg)
{
    pthread_mutex_unlock( &mutexid);
}


void* thread_function(void* arg)
{
    int i;
    pthread_cleanup_push( &cleanup_handler, NULL );
    for(i=0; i<5; i++) {
        pthread_mutex_lock(&mutexid);
        count++;
        printf("count = %d\n", count);
        sleep(1);
        pthread_mutex_unlock(&mutexid);
    }
    // 정상 동작일 때_cleanup() pop()하기!
    pthread_cleanup_pop( 0 );
}

int main(void)
{
    pthread_tthreads[NUMTHREADS];
    int i;
    for(i=0; i< NUMTHREADS; i++) {
    pthread_create(&threads[i],NULL,thread_function,NULL);
        }
    for(i=0; i< NUMTHREADS; i++) {
    pthread_join(threads[i], NULL);
        }
    return 0;
}