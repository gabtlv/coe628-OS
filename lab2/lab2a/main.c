#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* cd dist/Debug/GNU-Linux
 * ln lab2a hello
 * ln lab2a goodbye
*/
#define TOO_FEW_ARGS 1
#define TOO_MANY_ARGS 2


int main(int argc, char* argv[]) {
    int exit_code = EXIT_SUCCESS;
    char *greeting = "Hello";
    char *person = "UNKNOWN";


    // Exit code
    if (argc == 1) {
        exit_code = TOO_FEW_ARGS;
    } else if (argc == 2) {
        exit_code = EXIT_SUCCESS;
        person = argv[1];
    } else {
        exit_code = TOO_MANY_ARGS;
        person = argv[1];   // still print first name
    }


    //Check last 3 characters
    int len = strlen(argv[0]);
    if (len >= 3) {
        char *last3 = argv[0] + len - 3;
        if (strcmp(last3, "bye") == 0) {
            greeting = "Bye";
        }
    }


    printf("%s %s\n", greeting, person);
    return exit_code;
}
