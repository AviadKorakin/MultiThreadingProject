#ifndef THREADPOOL_H
#define THEREADPOOL_H


#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "Queue.h"
typedef struct {
    void * (*task_function)(void*); // Function pointer for the task function
    void* task_data; // Data associated with the task
} Task;

// Thread pool data structure
typedef struct {
    pthread_t * threads;
    int numberOfThreads;
    Queue * task_queue;
    int num_tasks;
    pthread_mutex_t queue_mutex;
    pthread_cond_t queue_empty_cond;
    pthread_mutex_t queue_mutex_full;
    pthread_cond_t queue_full_cond;
    int done;
    int numberOfFinishers;
} ThreadPool;

void thread_pool_init(ThreadPool* pool,int numberOfThreads);
void thread_pool_add_task(ThreadPool* pool, void * (*task_function)(void*), void* task_data);
void* thread_worker(void* arg);
void thread_pool_terminate(ThreadPool* pool);
#endif/* THREADPOOL_H */
