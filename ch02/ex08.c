#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>

void openfile_test(void);
int main(void)
{
    struct rlimit mylimit;

    getrlimit(RLIMIT_NPROC, &mylimit);
    printf("Current Number of Process : softlimit=%ld, hardlimit=%ld\n",\
        mylimit.rlim_cur, mylimit.rlim_max);
    
    getrlimit(RLIMIT_NOFILE, &mylimit);
    printf("\nCurrentNumber of File : softlimit=%ld, hardlimit=%ld\n",\
        mylimit.rlim_cur, mylimit.rlim_max);
    
    mylimit.rlim_cur= 5;
    setrlimit(RLIMIT_NOFILE, &mylimit);
    openfile_test();
    return 0;
}

void openfile_test(void)
{
    char *filename[6] = { "test1", "test2", "test3", "test4", "test5", "test6" };
    FILE *fp;
    int i;
    for(i=0; i<6; i++)
    {
        if((fp=fopen(filename[i], "w"))==NULL) {
            perror(filename[i]);
            exit(1);
        }
    }   
    return;
}
