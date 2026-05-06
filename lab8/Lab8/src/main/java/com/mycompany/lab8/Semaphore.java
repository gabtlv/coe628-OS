package com.mycompany.lab8;

public class Semaphore {

    private int value; // stores the number of available resources 
    
    // value tracks how many threads can entre 
    // value = 0 no resources --> must wait 
    // value = 1 one resource available 

    public Semaphore(int value) {
        this.value = value; // setting the initial value in the constructor
        // different sema[hores need different starting value s
    }
    // full.up() chef says food is placed on the table
    // full.down() customner removes the food 
    public synchronized void up() { // the release operation is defined, SYNCHRONIZED bc one tread can execute this method at a time  
        value++; // increases available resource, since a thread is finished the resource is available  
        notify(); // wakes up ONE waitiing thread (if a thread was blocked in down, it can enter now)
    }

    // full.down() customer wants a food item on the table they ask
    //empty.down() chef wants to place food on the table
    public synchronized void down() { // defines the acquire operation
        while (value == 0) { // if this is the case, cannot proceed, thread must wait 
            try {
                wait(); // puts the thread to sleep / blocked 
            } catch (InterruptedException ex) {
                Thread.currentThread().interrupt();
            }
        }
        value--; // thread is now entering the critical section, so s = --  
    }
}