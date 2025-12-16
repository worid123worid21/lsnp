#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(void)
{
    pid_t pid;
    int status;

    switch (pid=fork())
    {
        case -1:
            perror("fork");
            break;
    
        case 0:
            printf("CHILD PROCESS: %d\n", getpid());
            sleep(60);
            system("echo CHILD");
            system("ps -l");
            exit(0);
            break;

        default:
            printf("PARENT PROCESS: %d\n", getpid());
            printf("Return Value of fork: %d\n\n", pid);

            wait(&status);
            if(WIFEXITED(status)){
                printf("Child Process is deaded by exit()\n");
            }else if(WIFSIGNALED(status)){
                printf("Child Process is deaded by signal()\n");
            }
            //sleep(7);

            system("echo PARENT");
            system("ps -l");
            break;
    }
    return 0;
}