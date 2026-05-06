#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <pthread.h>
#include <sys/stat.h>

// gcc -pthread main.c -o lab5

void * count(void *);
int globalNumber = 0;

// 1. Create and initialize the mutex
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

int main(void) {
    int i;
    pthread_t counterThread[5];

    // 2. Create the 5 threads
    for (i = 0; i < 5; i++) {
        if (pthread_create(&counterThread[i], NULL, count, NULL) != 0) {
            perror("Failed to create thread");
            return 1;
        }
    }

    // 3. Wait for all threads to finish
    for (i = 0; i < 5; i++) {
        pthread_join(counterThread[i], NULL);
    }

    printf("\nFinal Global Number: %d\n", globalNumber);

    return (0);
}

void * count(void * junk) {
    int loopCount = 0;
    
    while (loopCount < 10) {
        // 4. Critical Section Start
        pthread_mutex_lock(&mutex1); 
        
        int tmpNumber = globalNumber;
        printf("counter: %d, Thread: %ld, PID: %d\n",
                tmpNumber, (long)pthread_self(), getpid());
        
        tmpNumber++;
        usleep(random() % 2); // Artificial delay to encourage context switching
        globalNumber = tmpNumber;
        
        pthread_mutex_unlock(&mutex1);
        // 5. Critical Section End
        
        loopCount++;
    }
    return (NULL);
}