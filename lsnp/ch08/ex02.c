#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *thread_function(void* arg);

int main(void)
{
    int status;
    pthread_t a_thread;
    void *thread_resualt;

    status = pthread_create(&a_thread, NULL, thread_function, (void*)NULL);
    if(status!=0){
        perror("rthread_create");
        exit(1);
    }
    void* ret_value;
    sleep(2); // 
    pthread_cancel(a_thread);
    pthread_join(a_thread, &ret_value);// wait 함수 같은 느낌
    //printf("return from thread : %s\n", (char*) ret_value);
    return 0;
}

void* thread_function(void* arg) {
    //pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL); // 강제 종료 허용(기본값)
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
    printf("this is thread_function\n");
    sleep(10);
    printf("thread_functionis end\n");
    pthread_exit("Hello, Thread");
}