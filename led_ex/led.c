#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <dlfcn.h>
#define SW 5
#define CDS 0
#define LED 1
int state = 0;  // 0: OFF, 1: ON
pthread_mutex_t mutexid;
typedef void (* OP_FUNC) (void );
OP_FUNC fcds;
OP_FUNC fled;
void *cdsThread(void *arg){
void *handle;
   handle=dlopen("/home/suseok/wiring/libcdsControl.so", RTLD_LAZY);
   if(handle==NULL) {
      printf("%s\n", dlerror());
      exit(1);
   }
   fcds = (OP_FUNC) dlsym(handle, "cdsControl");
    fcds();
    dlclose(handle);
    return NULL;
}
void *ledThread(void *arg) {
    void *handle;
   handle=dlopen("/home/suseok/wiring/libledControl.so", RTLD_LAZY);
   if(handle==NULL) {
      printf("%s\n", dlerror());
      exit(1);
   }
   fled = (OP_FUNC) dlsym(handle, "ledControl");
    fled();
    dlclose(handle);
    return NULL;
}
int main() {
    wiringPiSetup();
    pinMode(SW, INPUT);
    pinMode(CDS, INPUT);
    pinMode(LED, OUTPUT);
    pthread_t tid1, tid2;
    pthread_mutex_init(&mutexid, NULL);
    pthread_create(&tid1, NULL, cdsThread, NULL);
    pthread_create(&tid2, NULL, ledThread, NULL);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_mutex_destroy(&mutexid);
    return 0;
}