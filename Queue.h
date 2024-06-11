#ifndef QUEUE_H
#define QUEUE_H

typedef struct Node {
    void * data;
    struct Node* next;
} Node;

typedef struct Queue {
    Node* front;
    Node* rear;
} Queue;

Queue* create_queue();
void  init_queue(Queue* queue);
int isempty(Queue* q);
void enqueue(Queue* queue, void* data);
void * dequeue(Queue* queue);
void free_queue(Queue* q);
#endif /* QUEUE_H */


