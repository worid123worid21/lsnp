#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>

//4 -> zombie 문제를 해결하기 위해 선언 및 등록
void child_handler(int signo)
{
	pid_t pid;
	while(1) {
		pid = waitpid(0, NULL, WNOHANG);
		if(pid==-1 && errno == ECHILD)
			break;
		else if(pid==-1) {
			perror("waitpid");
			abort();
		} else if(pid==0)
			break;
		printf("Return Value : %d\n", pid);
	}	
}