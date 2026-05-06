// if not already compiled run: (wont build in netbeans)
// gcc -pthread -o main main.c
// else if the main executable is already in folder you can just run it
#include <stdlib.h>     /* NULL */
#include  <stdio.h> /* printf */
#include  <sys/types.h> /* pid_t */
#include <unistd.h> /* get_pid */
#include <stdlib.h>     /* exit, EXIT_FAILURE */
#include <sys/wait.h> /* wait */
#include <pthread.h>
#include <time.h>

#define PRODUCER_NO 5 //Number of producers, can change here
#define NUM_PRODUCED 20 //2000 //Number of items to be produced, can change here

void *generator_function(void*);
void *print_function(void*); // these are both thread functions, just letting the compiler know of their existence
long sum; /* shared variable storing the sum of all numbers added by all threads*/
long finished_producers; // counts how many generator threads are done, lets print thread know when all workers are done

//C: Mutex declaration and initialization
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER; // creates and initializes a mutex (lock)
// only one thread can hold this lock at a time


//F: Condition variable declaration and initialization
pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER; // a condition variable is made
// this lets one thread sleep and wait until another thread signals that something happened
// this is added so that the print thread should not print sum too early it must wait for all generator threads to finish

int main(void) {
    int i; // loop counter
    pthread_t producers[PRODUCER_NO]; // array to store the 5 generator thread IDS
    pthread_t print_thread; // variable storing the print thread
   
    /* initialize random seed: */
    srand(time(NULL)); // initializes the random number generator
    sum = 0;
    finished_producers=0; // no producers have been finished yet initialized at 0
   
    //A: Creates five generator thread
    for (i = 0; i < PRODUCER_NO; i++){ // for each i from 0 to 4 create a thread and store its ID in producers[i]
        pthread_create(&producers[i], NULL, generator_function, NULL); // make the threads run generator_function
    }
   
    //D: Creates print thread
    pthread_create(&print_thread, NULL, print_function, NULL);
    // creating one more thread that runs the print_function, since part 2 says the printing of sum should happen in its own thread
   
    //B: Makes sure that all generator threads has finished before proceeding
    for (i = 0; i < PRODUCER_NO; i++) {  // for reach generator thread, don't continue until that thread is done.
        // without join, the main might exit before the workers are done
        pthread_join(producers[i], NULL);
    }
   
    //E: Makes sure that print thread has finished before proceeding
    pthread_join(print_thread, NULL); // main needs wait for the print thread too
    // so the whole program doesnt end before the final print happens
   

    return (0);

}

void * generator_function(void* junk) { // this is the function that each producer thread runs
    long counter = 0; // counts how many times this thread has produced, a loop counter
    long sum_this_generator = 0; // keeps track of how much this one thread contributed
   
    while (counter < NUM_PRODUCED) { // this thread repeats until it has produced 20 items
        long rnd_number = 1;// right now the produced number is always 1 instead of random
        // can make = 1, output should be 100 (20 loops*5 threads)
     
        pthread_mutex_lock(&mutex1); // this locks the shared resource section
        long tmpNumber = sum; // copies the current sum into a temporary variable since we want to show the current sum before the addition
        printf("current sum of the generated number up to now is %ld going to add %ld to it.\n", tmpNumber, rnd_number);
        sum = tmpNumber + rnd_number; // performs the actual addition to the shared variable
        pthread_mutex_unlock(&mutex1); // this unlocks the shared resource section
       
        counter++; // this thread completed one production
        sum_this_generator += rnd_number; // add this produced value to the thread's local total
        usleep(1000);
    }
    // when this generator is done it prints its own total, each should print 20
    printf("--+---+----+----------+---------+---+--+---+------+----\n");
    printf("The sum of produced items for this number generator at the end is: %ld \n", sum_this_generator);
    printf("--+---+----+----------+---------+---+--+---+------+----\n");
   
    pthread_mutex_lock(&mutex1); // lock the mutex again
    // now this lock is for updating finished_producers
    // B/C multiple threads finishing at the same time could mess it up
    finished_producers++; // marking that one more producer thread finished
   
    //H: If all generator has finished fire signal for condition variable
    if (finished_producers == PRODUCER_NO) { // if this was the last producer WAKE UP PRINT THREAD
        pthread_cond_signal(&cond1); // B/c PRINT thread is waiting for all producers to finish
    }
   
    pthread_mutex_unlock(&mutex1); // release the lock after updating the finished_producers count so that next thread can be added

    return 0;
}

void *print_function(void* junk) {
    pthread_mutex_lock(&mutex1); // this is the thread that prints the final answer
    // locks before checking shared variable FINISHED_PRODUCERS because thats also shared data, one thread could be changing it or read it at the same time
    // otherwise there could be a race condition
    /* imagine this happens:
     finished producers = 4
     the print thread checks < 5
     BUT AT THE SAME TIME A GENERATOR THREAD FINISHES AND INCREMENTS IT TO 5
     the print thread would miss that update and sleep forever
     deadlock */
    //G: Wait until all generator has finished
    while (finished_producers < PRODUCER_NO) {  // keep waiting until all 5 producers are done
        pthread_cond_wait(&cond1, &mutex1);
    }
   
    // then finally we can print the total value
   
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("The value of counter at the end is: %ld \n", sum);
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
   
    pthread_mutex_unlock(&mutex1); // unlock the mutex after printing
    return(0);
}