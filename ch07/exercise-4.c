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
    int shmid, semid;
    char* shmaddr;

    if ((shmid = shmget(0x123400, 30, 0660 | IPC_CREAT)) == -1) {
        perror("shmget");
        exit(1);
    }

    if ((semid = semget(0x567800, 2, 0660 | IPC_CREAT)) == -1) {
        perror("semget");
        exit(1);
    }

    if ((shmaddr = shmat(shmid, NULL, 0)) == (char*)-1) {
        perror("shmat");
        exit(1);
    }

    while (1) {
        sem_op(semid, 1, -1); // read lock
        if (!strcmp(shmaddr, "end")) {
            sem_op(semid, 0, +1); // 마지막에 write unlock
            break;
        }
        printf("recv : %s\n", shmaddr);
        sem_op(semid, 0, +1); // write unlock
    }

    shmdt(shmaddr);
    shmctl(shmid, IPC_RMID, NULL);

    semctl(semid, 0, IPC_RMID, 1); //메모리 강제 종료
    semctl(semid, 1, IPC_RMID, 0); //     //

    return 0;
}
