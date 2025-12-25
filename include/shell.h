#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LINE 80
#define MAX_HISTORY 10
#define min(a,b) ((a) < (b) ? (a) : (b))

void shell_cd(char **args);
void shell_pwd(void);
void shell_help(void);
void shell_history(char history[MAX_HISTORY][MAX_LINE], int history_count);

#endif