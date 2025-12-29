#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <sys/wait.h>
#include <unistd.h>

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

//함수 3가지 선언, 프로그램 수행 중, 외부 신호는 일단 블록한다.
// siguser1() -> SIGUSR1   10사용자 정의 신호?
// siguser2() -> SIGUSR2   12사용자 정의 신호?
// interrupt()

int main()
{
	struct sigaction act;
	
	act.sa_handler = child_handler;
	sigfillset(&act.sa_mask);
	act.sa_flags = SA_RESTART;
	sigaction(SIGCHLD, &act, NULL);

	switch (fork()){
	case -1:
		break;
	case 0:
		printf("CHILD PID : %d\n", getpid());
		execlp("sleep", "sleep", "5", NULL);
		perror("execlp");
		exit(1);
		break;
	default:
		while(1);
		break;
	}
    return 0;
}