package com.mycompany.lab8;

import java.util.logging.Level;
import java.util.logging.Logger;

public class ProducerConsumer {

    static final int BUFFER_SIZE = 1; // table has space for one item 
    static Semaphore full = new Semaphore(0); // semaphore full is created = 0 bc there is no food on the table 
    static Semaphore empty = new Semaphore(BUFFER_SIZE); // empty starts at 1 because the table/buffer has one empty slot at the start
    
    // full = 0 means there is space on the table, customner cannot eat since the chef didnt place anything down 
    // empty = 0 means there is no space on the table 

    public static void main(String[] args) {
        Queue q = new Queue(BUFFER_SIZE); // the table, creating shared buffer so that both producer and copnsumer can use
        Producer p = new Producer(q); // creating BOTH threads, they both need to access the same queue
        Consumer c = new Consumer(q);

        p.start();
        c.start(); // starting both threads, they run concurrently
    }

    public static class Queue { // the actual buffer object

        private final int[] numbers; // actual storage
        private int rear; // where prodcuer places item
        private int front; // where customer takes item 
        private final int size; // queue size 

        public Queue(int size) { // just initializing the queue
            numbers = new int[size + 1]; // avoids overflow confusion
            front = 0; // we start empty
            rear = 0;
            this.size = size + 1; // must match array size for circular indeixing 
        }

        public int get() { // gets an item from the front, customer takes item from table 
            int value = numbers[front];
            front = (front + 1) % size;
            return value;
        }

        public void put(int value) { // this adds an item to the rear of the queue, where chef places the food 
            numbers[rear] = value;
            rear = (rear + 1) % size;
        }
    }

    private static class Producer extends Thread {

        private final Queue q; // producer/chef needs to access the queue/table

        public Producer(Queue q) {
            this.q = q; // stores the producer's food 
        }

        @Override
        public void run() { // chef makes 10 plates total
            for (int i = 0; i < BUFFER_SIZE * 10; i++) {
                empty.down(); // chef asks if there is space on the table 
                // if empty = 1, it becomes 0 and chef continues 
                // if empty = 0, chef waits 
                System.out.println("Producing: " + i); // printing that food was placed 
                q.put(i); // actually place the food 

                try {
                    Thread.sleep((int) (Math.random() * 10));
                } catch (InterruptedException ex) {
                    Logger.getLogger(ProducerConsumer.class.getName()).log(Level.SEVERE, null, ex);
                }

                full.up(); // chef is done, food is available increment full since table is full
            }
        }
    }

    private static class Consumer extends Thread {

        private final Queue q;

        public Consumer(Queue q) {
            this.q = q;
        }

        @Override
        public void run() { // consumer will consume 10 items total 
            for (int i = 0; i < BUFFER_SIZE * 10; i++) {
                full.down(); // customer is asking if there is food on the table 
                // full = 1: becomes 0 and customer consumes
                // full = 0 customer must wait 
                System.out.println("\t\tConsuming: " + q.get());// take the food and print it 

                try {
                    Thread.sleep((int) (Math.random() * 10));
                } catch (InterruptedException ex) {
                    Logger.getLogger(ProducerConsumer.class.getName()).log(Level.SEVERE, null, ex);
                }

                empty.up(); // customer says that table is is free now which increases empty so the chef can place another item
            }
        }
    }
}