#include "../include/shell.h"

void shell_cd(char **args) {
    char *dir = args[1]; 
    if (args[1] == NULL) {
        dir = getenv("HOME");
    }
    
    if (dir == NULL) {
        fprintf(stderr, "cd: missing argument or HOME not set\n");
    } 
    else if (chdir(dir) != 0) {
        perror("cd failed");
    }
}

void shell_pwd(void) {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
    }
    else {
        perror("getcwd() error");
    }
}

void shell_help(void) {
    printf("--- UnixShell Help ---\n");
    printf("Type command names and arguments, and hit enter.\n");
    printf("Built-in commands:\n");
    printf("  cd <path>    : Change directory\n");
    printf("  pwd          : Print working directory\n");
    printf("  exit         : Exit shell\n");
    printf("  help         : Show this help message\n");
    printf("  history      : Show command history\n");
}

void shell_history(char history[MAX_HISTORY][MAX_LINE], int history_count) {
    int cmd_num = 1;

    if (history_count > MAX_HISTORY) {
        cmd_num = history_count - MAX_HISTORY + 1;
    }

    for (int i = (history_count % MAX_HISTORY); i < min(history_count, MAX_HISTORY); i++) {
        printf("%4d %s\n", cmd_num++, history[i]);
    }
    for (int i = 0; i < (history_count % MAX_HISTORY); i++) {
        printf("%4d %s\n", cmd_num++, history[i]);
    }
}