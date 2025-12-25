#include "../include/shell.h"

char history[MAX_HISTORY][MAX_LINE];
int history_count = 0;

int main(void) {
    char *args[MAX_LINE/2 + 1];
    int should_run = 1;
    
    while (should_run) {
        while (waitpid(-1, NULL, WNOHANG) > 0);

        printf("uinxsh> ");
        fflush(stdout);
        
        char input[MAX_LINE];
        if (fgets(input, MAX_LINE, stdin) == NULL) {
            break;
        }

        input[strcspn(input, "\n")] = 0;

        if (strlen(input) == 0) continue;

        if (strcmp(input, "!!") == 0) {
            if (history_count == 0) {
                printf("No commands in history \n");
                continue;
            }
            int last_index = (history_count - 1) % MAX_HISTORY;
            strcpy(input, history[last_index]);
            printf("%s\n", input);
        }

        strcpy(history[history_count % MAX_HISTORY], input);
        history_count++;

        int i = 0;
        char *token = strtok(input, " ");
        while (token != NULL) {
            args[i] = token;
            token = strtok(NULL, " ");
            i++;
        }
        
        int background = 0;
        if (i > 0 && strcmp(args[i-1], "&") == 0) {
            background = 1;
            args[i-1] = NULL;
        }
        else {
            args[i] = NULL;
        }

        if (args[0] == NULL) {
            continue;
        }


        if (strcmp(args[0], "exit") == 0) {
            should_run = 0;
            continue;
        }

        if (strcmp(args[0], "cd") == 0) {
            shell_cd(args);
            continue;
        }

        if (strcmp(args[0], "pwd") == 0) {
            shell_pwd();
            continue;
        }

        if (strcmp(args[0], "help") == 0) {
            shell_help();
            continue;
        }

        if (strcmp(args[0], "history") == 0) {
            shell_history(history, history_count);
            continue;
        }

        int pipe_idx = -1;
        for (int j = 0; args[j] != NULL; j++) {
            if (strcmp(args[j], "|") == 0) {
                pipe_idx = j;
                break;
            }
        }

        if (pipe_idx != -1) {
            args[pipe_idx] = NULL;
            char **args_right = &args[pipe_idx + 1];

            int pipefd[2];
            if (pipe(pipefd) == -1) {
                perror("pipe failed");
                return 1;
            }

            int pid1 = fork();
            if (pid1 == 0) {
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[0]); 
                close(pipefd[1]);
                if (execvp(args[0], args) == -1) {
                    perror("Execution failed (left)");
                    exit(1);
                }
            }

            int pid2 = fork();
            if (pid2 == 0) {
                dup2(pipefd[0], STDIN_FILENO);
                close(pipefd[0]);
                close(pipefd[1]);
                if (execvp(args_right[0], args_right) == -1) {
                    perror("Execution failed (right)");
                    exit(1);
                }
            }

            close(pipefd[0]);
            close(pipefd[1]);
            waitpid(pid1, NULL, 0);
            waitpid(pid2, NULL, 0);
            continue;
        }

        pid_t pid = fork();

        if (pid < 0) {
            fprintf(stderr, "Fork Failed\n");
            return 1;
        } 
        else if (pid == 0) {
            if (execvp(args[0], args) == -1) {
                printf("Command not found: %s\n", args[0]);
            }
            exit(0);
        } 
        else {
            if (background == 1) {
                printf("[Running in background] PID: %d\n", pid);
            }
            else {
                waitpid(pid, NULL, 0);
            }
        }
    }
    
    return 0;
}