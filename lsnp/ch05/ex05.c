#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    int status;
    switch(fork())
    {
        case-1:
            perror("fork");
            exit(1);
            break;
        case 0:
            sleep(3);
            exit(0);
            //exit(1);
            break;
        default:
            wait(&status);
            if(WIFEXITED(status)) {
                printf("exit code of child process : %d\n", WEXITSTATUS(status));
            }
        }
}