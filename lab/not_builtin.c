#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

void redirect_out(char** arg);
int is_pipe(char** arg);

int not_builtin(char** arglist)
{
    pid_t pid;
    int qwe; //3

    //3 백그라운드 실행
    for(int i =0; arglist[i] != NULL; i++){
        if(strcmp(arglist[i], "&")==0){
            qwe = WNOHANG;
            arglist[i] = NULL;
        }else{
            qwe = 0;
        }
    }

    switch (pid=fork())
    {
        case -1:
            perror("fork");
            break;
    
        case 0:
            
            signal(SIGINT, SIG_DFL);// 자식은 기본 핸들러로 다시 설정
            signal(SIGQUIT, SIG_DFL);// 위와 같음

            //5 pipe 추가
            // int pipe_idx = -1;
            // for(int i =0; arglist[i] != NULL; i++){
            //     if(strcmp(arglist[i], "|")==0){
            //         // |이전 명령어, 이후 명령어 실행    
            //         pipe_idx = i;
            //         break;
            //     }else{
            //         qwe = 0;
            //     }
            // }
            // 5 

            //4
            redirect_out(arglist);
            
            //5 pipe
            // int s = is_pipe(arglist);

            // if (s){
            //     int pd[2];
            //     pid_t pid;
            //     int n;
            // }
            
            // if(pipe(pid) == -1 ) {
            //     perror("pipe");
            //     exit(1);
            // }
            //     switch(pid=fork()) {
            //     case -1:
            //         perror("fork");
            //         exit(1);
            //     break;

            //     case 0:
            //         close(pd[0]);
            //         dup2(pd[1], 1);
            //         close(pd[1]);
            //         execvp(arglist[0], arglist);
            //         perror("execvp1");
            //         exit(1);
            //         break;

            //     default:
            //         close(pd[1]);
            //         dup2(pd[0], 0);
            //         close(pd[0]);
            //         execvp(arglist[s], &arglist[s]);
            //         perror("execvp2");
            //         break;
            //     }
            // }
            //5

            //5 pipe
            int s = is_pipe(arglist);

            if (s) {
                int pd[2];
                pid_t pid;

                if (pipe(pd) == -1) {
                    perror("pipe");
                    exit(1);
                }

                switch (pid = fork()) {
                case -1:
                    perror("fork");
                    exit(1);

                case 0:
                    close(pd[0]);
                    dup2(pd[1], 1);
                    close(pd[1]);
                    execvp(arglist[0], arglist);
                    perror("execvp1");
                    exit(1);

                default:
                    close(pd[1]);
                    dup2(pd[0], 0);
                    close(pd[0]);
                    execvp(arglist[s], &arglist[s]);
                    perror("execvp2");
                    exit(1);
                }
            }


            execvp(arglist[0], arglist);
            perror("execv 오류");// -1 값이 아닌데 왜 씀? -> 성공하면 파일이 ls로 바뀌어서 prror() 없어짐, 실패면 원하는 대로 perror() 수행         
            exit(0);
            break;

        default:
            //wait(NULL); // 보통 shell 프로그램 종료까지 기달려서 wait() 가능
            // 우리의 실습도 shell
            waitpid(-1, NULL, qwe);//3
    }
    
    return 0;
}


//4
void redirect_out(char** arg)
{
    int out_flag = 0;
    for(int i =0; arg[i] != NULL; i++){
        if(strcmp(arg[i], ">")==0 || strcmp(arg[i], "1>")==0){
            arg[i] = NULL;
            out_flag = 1;
            continue;
        }

        if(out_flag==1){
            int fd;
            // if(arglist[i]==NULL){
            //     printf("파일 없음 확인 다시 ㄱㄱ\n");
            //     return;
            // }

            if((fd=open(arg[i], O_WRONLY|O_CREAT|O_TRUNC,0666))==-1){
                perror("open");
                exit(1);
            }
            close(1);
            dup(fd);
            close(fd);
        }
    }
    
}

//5 pipe
int is_pipe(char** arg)
{
     for(int i =0; arg[i] != NULL; i++){
        if(strcmp(arg[i], "|")==0){
            arg[i] = NULL;
            return i + 1;
        }
    }

    return 0;
}