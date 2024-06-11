#include "ThreadPool.h"
#define MAX_TASKS 256

void thread_pool_init(ThreadPool* pool, int numberOfThreads) //init
{
	pool->numberOfThreads = numberOfThreads;
	pool->threads = malloc(sizeof(pthread_t)* numberOfThreads);
	pool->task_queue = create_queue();
	pool->num_tasks = 0;
	pool->done = 1;
	pool->numberOfFinishers = 0;
	pthread_mutex_init(&pool->queue_mutex, NULL);
	pthread_mutex_init(&pool->queue_mutex_full, NULL);
	pthread_cond_init(&pool->queue_empty_cond, NULL);
	pthread_cond_init(&pool->queue_full_cond, NULL);

	//worker threads
	for (int i = 0; i < numberOfThreads; i++) {
		pthread_create(&pool->threads[i], NULL, thread_worker, pool);
	}
}

void thread_pool_add_task(ThreadPool* pool, void * (*task_function)(void*), void* task_data) {
	while (pool->num_tasks >= MAX_TASKS) {
		pthread_cond_wait(&pool->queue_full_cond, &pool->queue_mutex_full);//overload
	}
	pthread_mutex_unlock(&pool->queue_mutex_full);
	// Add the task to the queue
	Task * newTask = malloc(sizeof(Task));
	newTask->task_function = task_function;
	newTask->task_data = task_data;
	pthread_mutex_lock(&pool->queue_mutex);
	enqueue(pool->task_queue, (void *)newTask);
	pool->num_tasks++;
	// Signal the worker threads that a new task is available
	pthread_cond_signal(&pool->queue_empty_cond);
	pthread_mutex_unlock(&pool->queue_mutex);
}

void* thread_worker(void* arg) {//workers
	ThreadPool* pool = (ThreadPool*)arg;

	while (1) {
		pthread_mutex_lock(&pool->queue_mutex);

		// Wait until there's a task in the queue
		while (pool->num_tasks == 0 && pool->done) {
			pthread_cond_wait(&pool->queue_empty_cond, &pool->queue_mutex);
		}
		//waking in the critcial section
		if (!pool->done && pool->num_tasks == 0)
		{
			if (pool->numberOfFinishers == pool->numberOfThreads - 1)
			{
				pthread_mutex_destroy(&pool->queue_mutex);
				pthread_cond_destroy(&pool->queue_empty_cond);
				pthread_exit(NULL);
			}
			pool->numberOfFinishers++;
			pthread_cond_signal(&pool->queue_empty_cond);//waking the others.
			pthread_mutex_unlock(&pool->queue_mutex);
			pthread_exit(NULL);
		}
		// Get the task from the queue
		Task * task = (Task *)dequeue(pool->task_queue);//critical section
		pool->num_tasks--;
		pthread_mutex_unlock(&pool->queue_mutex);
		if (pool->num_tasks < MAX_TASKS / 5)
			pthread_cond_signal(&pool->queue_full_cond);//waking the creator.
		task->task_function(task->task_data);
		free(task);
	}
}
void thread_pool_terminate(ThreadPool* pool)
{
	pool->done = 0;//finish announcement
	pthread_cond_broadcast(&pool->queue_empty_cond);//finish announcement
	for (int i = 0; i < pool->numberOfThreads; i++) {
		pthread_join(pool->threads[i], NULL);
	};
	free(pool->threads);
}

