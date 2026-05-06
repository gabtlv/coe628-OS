/*
 * =========================================================
 * 1. Single Command:       ls
 * 2. Multi-word:           ls -l
 * 3. Background (&):       sleep 5&   (Prompt returns immediately)
 * 4. Path test:            pwd
 * 5. Error test:           lsssss     (Should show error message)
 * 6. Built-in command:     exit
 * =========================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

int main(int argc, char** argv) {
    char command[100];
    char *argPtr[100];
    int ch;

    while (1) {
        int index = 0;
        int argPtrNum = 0;
        bool ampFlag = false;

        printf("Your command> ");
        fflush(stdout);

        // Step 1: Read input character-by-character
        while ((ch = getchar()) != '\n' && ch != EOF) {
            command[index] = (char) ch;

            // Step 2: Replace spaces with '\0'
            if (command[index] == ' ') {
                command[index] = '\0';
            }
            // Start of a new word
            else if (index == 0 || command[index - 1] == '\0') {
                argPtr[argPtrNum] = &command[index];
                argPtrNum++;
            }
            index++;
        }

        // Handle empty input
        if (index == 0) {
            continue;
        }

        // Step 3: Null-terminate the final word
        command[index] = '\0';

        // Step 4: Background execution check
        if (index > 0 && command[index - 1] == '&') {
            ampFlag = true;
            command[index - 1] = '\0';

            // Remove empty argument caused by '&'
            if (argPtrNum > 0 && strlen(argPtr[argPtrNum - 1]) == 0) {
                argPtrNum--;
            }
        }

        // Step 5: NULL-terminate argv array
        argPtr[argPtrNum] = NULL;

        // ===== BUILT-IN EXIT COMMAND =====
        if (strcmp(argPtr[0], "exit") == 0) {
            printf("Exiting shell...\n");
            break;
        }
        // =================================

        // Step 6: Create child process
        pid_t pid = fork();

        if (pid < 0) {
            perror("Fork failed");
            exit(1);
        }
        else if (pid == 0) {
            // CHILD PROCESS
            if (execvp(argPtr[0], argPtr) < 0) {
                perror("Command execution failed");
                exit(1);
            }
        }
        else {
            // PARENT PROCESS
            if (!ampFlag) {
                waitpid(pid, NULL, 0);
            }
            else {
                printf("[Process running in background with PID %d]\n", pid);
            }
        }
    }

    return EXIT_SUCCESS;
}
