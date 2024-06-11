#include "Queue.h"
#include <stdlib.h>
#include <string.h>

Queue* create_queue() {
	Queue* queue = (Queue*)malloc(sizeof(Queue));
	if (queue) {
		queue->front = NULL;
		queue->rear = NULL;
	}
	return queue;
}
void init_queue(Queue* q) {
	q->front = NULL;
	q->rear = NULL;
}

// Function to check if the queue is empty
int isempty(Queue* q) {
	return q->front == NULL;
}

// Function to enqueue a new element into the queue
void enqueue(Queue* q, void* data) {
	Node* new_node = (Node*)malloc(sizeof(Node));
	if (new_node) {


		new_node->data = data;
		new_node->next = NULL;

		if (isempty(q)) {
			q->front = new_node;
			q->rear = new_node;
		}
		else {
			q->rear->next = new_node;
			q->rear = new_node;
		}
	}
}

// Function to dequeue an element from the queue
void* dequeue(Queue* q) {
	if (isempty(q)) {
		return NULL;
	}

	Node* front_node = q->front;
	void* data = front_node->data;

	q->front = q->front->next;
	free(front_node);

	if (q->front == NULL) {
		q->rear = NULL;
	}

	return data;
}

// Function to free the queue
void free_queue(Queue* q) {
	while (!isempty(q)) dequeue(q);
}

