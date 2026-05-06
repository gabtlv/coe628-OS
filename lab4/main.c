//ls | wc -w
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

void executecmd(char *cmd) {
    char *args[64];
    int i = 0;

    char *token = strtok(cmd, " ");
    while (token != NULL) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;

    execvp(args[0], args);
    perror("execvp failed");
    exit(1);
}

int main() {
    char input[256];

    while (1) {
        printf("Your Command> ");
        fflush(stdout);

        if (fgets(input, sizeof(input), stdin) == NULL)
            break;

        input[strcspn(input, "\n")] = 0;

        if (strcmp(input, "exit") == 0) {
            printf("Exiting shell...\n");
            break;
        }

        // Check if pipe exists
        char *pipePos = strchr(input, '|');

        // ===============================
        // CASE 1: NO PIPE
        // ===============================
        if (pipePos == NULL) {
            pid_t pid = fork();

            if (pid == 0) {
                executecmd(input);
            }
            else {
                waitpid(pid, NULL, 0);
            }
        }

        // ===============================
        // CASE 2: HAS PIPE
        // ===============================
        else {
            char *left_cmd = strtok(input, "|");
            char *right_cmd = strtok(NULL, "|");

            int pipefd[2];
            pipe(pipefd);

            pid_t pid1 = fork();
            if (pid1 == 0) {
                close(pipefd[0]);
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[1]);
                executecmd(left_cmd);
            }

            pid_t pid2 = fork();
            if (pid2 == 0) {
                close(pipefd[1]);
                dup2(pipefd[0], STDIN_FILENO);
                close(pipefd[0]);
                executecmd(right_cmd);
            }

            close(pipefd[0]);
            close(pipefd[1]);

            waitpid(pid1, NULL, 0);
            waitpid(pid2, NULL, 0);
        }
    }

    return 0;
}