#include "minishell.h"

BT_TYPE btn_cmds[] = {
    "hostname", hostname_function,
    "cd", cd_function,//2
    "pwd", pwd_function,//2
    NULL, NULL
};

int is_builtin(char** arglist)
{
    for(int i=0; btn_cmds[i].cmd!=NULL; i++)
        if(strcmp(btn_cmds[i].cmd, arglist[0])==0) {
            btn_cmds[i].bt_func(arglist);
            return 1;        // builtin command
        }  
    return 0;       // NOT builtin command
}

void hostname_function(char** arg)
{
    char hostname[1024];
    gethostname(hostname,1023);
    printf("hostname : %s\n", hostname);
}

//2
void cd_function(char** arg)
{
    char *dir;

    if(arg[1]==NULL){
        dir = getenv("HOME");
    }else{
        dir = arg[1];
    }

    if(chdir(dir)==-1){
        perror("chdir");
        return;
    }
}

//2
void pwd_function(char** arg)
{
    char path[1024];

    getcwd(path, 1023);
    printf("%s\n", path);
}