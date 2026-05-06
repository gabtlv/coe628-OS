#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

/* ./lab2b abcd
 * ./lab2b WXYZ
 * ./lab2b abcd & ./lab2b WXYZ
*/
#define N_REPS 50
#define DEFAULT_SLOWDOWN 10000


int main(int argc, char *argv[]) {
    int i;
    int slow_down = DEFAULT_SLOWDOWN;
    if (argc == 1) {
        fprintf(stderr, "Usage: %s string [delay]\n", argv[0]);
        return 1;
    }
    if (argc >= 3) {
        slow_down = atoi(argv[2]);
        if (slow_down <= 0) slow_down = 1;   // prevent % 0
    }
    // Seed randomness (portable)
    srand((unsigned int)(time(NULL) ^ (unsigned int)getpid()));


    for (i = 0; i < N_REPS; i++) {


        // Acquire lock (busy-wait until mkdir succeeds)
        while (system("mkdir junk 2>/dev/null") != 0) ;


        // Critical section: print one full copy of the string
        char *cp = argv[1];
        while (*cp) {
            printf("%c", *cp++);
            fflush(stdout);
            usleep((unsigned int)(rand() % slow_down));
        }
        // Release lock
        system("rmdir junk 2>/dev/null");
        usleep(5000);
    }
    return EXIT_SUCCESS;
}
