#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void usr1_handler(int signo)
{
	printf("pid=%d received SIGUSR1, exiting.\n", getpid());
	exit(EXIT_SUCCESS);
}

void processInfo(char *msg)
{
	printf("%s: my pid = %d, ppid = %d, pgid = %d, sid = %d\n",
	       msg, getpid(), getppid(), getpgid(0), getsid(0));
}

int main(void)
{
	struct sigaction act;
	act.sa_handler=usr1_handler;
	sigfillset(&act.sa_mask);
	act.sa_flags=SA_RESTART;
	sigaction(SIGUSR1, &act, NULL);
	processInfo("main");
	pid_t cpid1, cpid2, cpid3;
	switch(cpid1=fork()) {
	case -1:
		perror("fork");
		break;
	case 0:
		processInfo("child1");
		sleep(5);
		printf("child1 exiting\n");
		exit(EXIT_SUCCESS);
		break;
	default:
		switch(cpid2=fork()) {
		case -1:
			perror("fork2");
			break;
		case 0:
			if(setpgid(0, 0) == -1) // 새로운 그룹 만들고 리더가 됨. 본인 pid값으로 만든다
				perror("setpgid error:");
			processInfo("child2");
			switch(cpid3=fork()) {
			case -1:
				perror("fork3");
				break;
			case 0:
				processInfo("child3");
				sleep(3);
				//kill(getpid(), SIGUSR1);
				kill(-getpgid(getpid()), SIGUSR1);
				printf("child3 exiting\n");
				exit(EXIT_SUCCESS);
				break;
			default:
				wait(NULL);
				printf("child2 exiting\n");
				exit(EXIT_SUCCESS);
				break;
			}
		}
	}
	sleep(2);
	//kill(0, SIGUSR1);
	waitpid(cpid2,NULL, 0);
	waitpid(cpid1,NULL, 0);
	printf("main exiting\n");
	return 0;
}