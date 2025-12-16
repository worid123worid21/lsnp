#include <sys/types.h>
#include <sys/ipc.h> // 공유 메모리 사용 시 필요
#include <sys/msg.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/sem.h> //세마포어 추가

// semaphore
// 초기값 설정도 매우 중요
//int semget(key_t key,  int num_sems,  int sem_flags);
// int cnt =0;
// static int semid;

int main()
{
	int shmid, i,j ;
	char *shmaddr;// 공유 메모리 주소

	if((shmid=shmget(0x123400, 30, 0660|IPC_CREAT|IPC_EXCL))==-1) {
		if((shmid=shmget(0x123400, 30, 0660))==-1){
			perror("shmget");
			exit(1);
		}
	}
 	if((shmaddr=shmat(shmid, (char *)0, 0))== NULL) {
		perror("shmat");
		exit(1);
	}
 	for(i=0; i<20; i++) {
 		sprintf(shmaddr, "shared memory test %d", i+1);
 		printf("send : %s\n", shmaddr);
 		for(j=0; j<100000000; j++);
 	}
 	sprintf(shmaddr, "end");

	if(shmdt(shmaddr)==-1 ) {
		perror("shmdt");
		exit(1);
	}

	return 0;
}
