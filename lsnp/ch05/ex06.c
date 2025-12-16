#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(void)
{
    pid_t pid;

    switch (pid=fork())
    {
        case -1:
        
            perror("fork");
            break;
    
        case 0:

            printf("CHILD PROCESS: %d\n", getpid());
            execlp("ls", "ls", "-l", (char*)NULL);
            perror("exexlp");// -1 값이 아닌데 왜 씀? -> 성공하면 파일이 ls로 바뀌어서 prror() 없어짐, 실패면 원하는 대로 perror() 수행         
            exit(0);
            break;

        default:

            wait(NULL);
            printf("child is dead\n");

            // printf("PARENT PROCESS: %d\n", getpid());
            // printf("Return Value of fork: %d\n\n", pid);
            // sleep(7);
            // system("echo PARENT");
            // system("ps -l");

            break;

    }
    return 0;
}