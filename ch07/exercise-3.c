#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void sem_op(int semid, int semnum, int op) {
    struct sembuf sb = { semnum, op, SEM_UNDO };
    if (semop(semid, &sb, 1) == -1) {
        perror("semop");
        exit(1);
    }
}

int main() {
    int shmid, semid, i;
    char* shmaddr;

    // 공유 메모리 생성/접근
    if ((shmid = shmget(0x123400, 30, 0660 | IPC_CREAT)) == -1) {
        perror("shmget");
        exit(1);
    }

    // 세마포어 2개 생성 (write=0, read=1)
    if ((semid = semget(0x567800, 2, 0660 | IPC_CREAT)) == -1) {
        perror("semget");
        exit(1);
    }

    // 초기화: write=1, read=0
    semctl(semid, 0, SETVAL, 1);
    semctl(semid, 1, SETVAL, 0);

    if ((shmaddr = shmat(shmid, NULL, 0)) == (char*)-1) {
        perror("shmat");
        exit(1);
    }

    for (i = 0; i < 20; i++) {
        sem_op(semid, 0, -1); // write lock
        sprintf(shmaddr, "shared memory test %d", i + 1);
        printf("send : %s\n", shmaddr);
        sem_op(semid, 1, +1); // read unlock
    }

    sem_op(semid, 0, -1);
    sprintf(shmaddr, "end");
    sem_op(semid, 1, +1);

    shmdt(shmaddr);

    semctl(semid, 0, IPC_RMID, 1); //메모리 강제 종료 -> 한쪽인지 양쪽인지 비교
    semctl(semid, 1, IPC_RMID, 0); //     //

    return 0;
}
