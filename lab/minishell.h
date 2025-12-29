#ifndef __MINISHELL_H__
#define __MINISHELL_H__

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h> //4  파일 중첩

//#include "minishell.h"

char** command_parse(char*);
void command_freelist(char **arglist);

typedef struct bt_type {
	char *cmd;
	void (*bt_func)(char **);
} BT_TYPE;

void hostname_function(char** arg);
void cd_function(char** arg);//2
void pwd_function(char** arg);//2
int is_builtin(char** arglist);
int not_builtin(char** arglist);//3
void child_handler(int signo); //4

#endif // __MINISHELL_H__