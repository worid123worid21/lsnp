#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

/* 시그널 카운터 */
volatile sig_atomic_t usr1_cnt = 0;
volatile sig_atomic_t usr2_cnt = 0;

/* SIGUSR1 handler */
void sigusr1_handler(int signo)
{
    usr1_cnt++;
}

/* SIGUSR2 handler */
void sigusr2_handler(int signo)
{
    usr2_cnt++;
}

/* SIGINT handler */
void interrupt_handler(int signo)
{
    printf("\nSIGUSR1 count : %d\n", usr1_cnt);
    printf("SIGUSR2 count : %d\n", usr2_cnt);
    exit(0);
}

int main(void)
{
    struct sigaction act;
    sigset_t block_set, wait_set;

    /* 1. 모든 시그널 블록 */
    sigfillset(&block_set);
    sigprocmask(SIG_SETMASK, &block_set, NULL);

    /* 2. SIGUSR1 등록 */
    act.sa_handler = sigusr1_handler;
    sigfillset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGUSR1, &act, NULL); // 필수로 알아야 한다.

    /* 3. SIGUSR2 등록 */
    act.sa_handler = sigusr2_handler;
    sigaction(SIGUSR2, &act, NULL);

    /* 4. SIGINT 등록 */
    act.sa_handler = interrupt_handler;
    sigaction(SIGINT, &act, NULL);

    printf("PID : %d\n", getpid());
    printf("Waiting for SIGUSR1, SIGUSR2 (Ctrl+C to exit)\n");

    /* 5. 대기 중 허용할 시그널만 설정 */
    sigfillset(&wait_set);
    sigdelset(&wait_set, SIGUSR1);
    sigdelset(&wait_set, SIGUSR2);
    sigdelset(&wait_set, SIGINT);

    /* 6. SIGINT 들어올 때까지 대기 */
    while (1)
       sigsuspend(&wait_set);

    // while(!end_flag);
    // printf("user1_count = %d, user2_count = %d\n",usr1_count, usr2_count);

    return 0;
}
