#include <stdlib.h>
#include <stdio.h>
#include "pri_queue.h"
/** @file pri_queue.c */
static Node_ptr_t head = NULL;
/**
 * Insert a Node into a priority queue.
 * @param priority
 * @param data
 * @author Gab Talavera
 */
void PQ_insert(int priority, char * data) {
 //FIX THIS
    Node_ptr_t new = malloc(sizeof(Node_t));
    new->priority = priority;
    new->data = data;
    new->next = NULL;
    

Node_ptr_t temp = malloc(sizeof(Node_t));
Node_ptr_t insertionNode = malloc(sizeof(Node_t));
Node_ptr_t currNode = malloc(sizeof(Node_t));


if(head == NULL){
    head = new;
    return;
} else {
    if(priority >= head->priority){
        temp->data = head->data;
        temp->priority = head->priority;
        temp->next = head->next;
        head = new;
        head->next = temp;
    }
    else{
        insertionNode = new;
        currNode = head;
        while(currNode->next != NULL && insertionNode->priority < currNode->next->priority){
            currNode = currNode->next;
        }
        if(currNode->next == NULL){
            currNode->next = insertionNode;
            insertionNode->next = NULL;
        } 
        else{
            temp = malloc(sizeof(Node_t));
            temp = currNode->next;
            currNode->next = insertionNode;
            insertionNode->next = temp;
        }
    }
}
}
/**
 * Delete and return the node with the highest priority.
 * @return The highest priority Node.
 */
Node_ptr_t PQ_delete() {
  //FIX THIS
    return NULL;
}

/**
 * Do NOT modify this function.
 * @return A pointer to the head of the list.  (NULL if list is empty.)
 */
Node_ptr_t PQ_get_head() {
    return head;
}

/**
 * Do NOT modify this function.
 * @return the number of items in the queue
 */
int PQ_get_size() {
    int size = 0;
    Node_ptr_t tmp;
    for(tmp = head; tmp != NULL; tmp = tmp->next, size++)
        ;
    return size;
}


