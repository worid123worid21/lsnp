#include<stdio.h>
#include<stdlib.h>

int main(void)
{
    FILE* fp;
    int m;
    if((fp=popen("grep -E \"Goodbye|Hello\"", "w")) == NULL) { //쓰기 모드 즉, 송신을 하겠다
    fprintf(stderr, "popenfailed.\n");
    exit(1);
   }
    for( m = 1; m <= 10; m++ ) {
        fprintf(fp, "Hello, pipe!!\n");
        fprintf(fp, "Goodbye, pipe...\n");
    }
    pclose(fp);
    return(0);
}