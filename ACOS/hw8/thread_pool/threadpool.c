#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <semaphore.h>
#include "threadpool.h"

int tp_init(thread_pool* tp, int cnt) {
	if (NULL == tp || cnt <= 0 || cnt >= 100) {
		if (NULL != tp)
			tp->alive = 0;
		fprintf(stderr, "Incorrect input parameters");
		return -1;
	}
	int retval;

	tp->threads = (pthread_t*)malloc(cnt * sizeof(pthread_t));
	if (NULL == tp->threads) {
		fprintf(stderr, "Threads allocation memory error\n");
		return -1;
	}
	tp->threads_cnt = cnt;

	if (0 != tp_queue_init(tp)) {
		fprintf(stderr, "Can't create job queue\n");
		return -1;
	}
	tp->alive = 1; 

	int i;
	for (i = 0; i < cnt; ++i) {
		if (0 != (retval = pthread_create(&(tp->threads[i]), NULL, worker_thread_routine, tp))) {
			fprintf(stderr, "Can't create thread [%d]\n", i);
			return retval;
		}	
	}
	return 0;	
}


int __tp_get_and_done(thread_pool* tp, task_func_t* routine, void** args) {

	if (NULL == tp || NULL == routine || NULL == args) {
		fprintf(stderr, "Invalid arguments\n");
		return -1;
	}
	
	if (0 != sem_wait(tp->queue->queueSem)) {
		perror("Waiting for semaphore");
		pthread_exit(NULL);
	}
	
	int retval;
	if (1 == tp->alive) {
		tp_node* job;
				
		if (0 != (retval = pthread_mutex_lock(&tp->queue->mutex))) {
			fprintf(stderr, "Mutex lock error\n");
			return retval;
		}
		
		job = tp_queue_get(tp);
		if (NULL == job) {
			fprintf(stderr, "Can't get job\n");
			if (0 != (retval = pthread_mutex_unlock(&tp->queue->mutex)))
				fprintf(stderr, "Mutex unlock error\n");
			return -1;
		}

		*routine = job->function;
		*args = job->args;

		if (0 != tp_queue_remove(tp)) {
			fprintf(stderr, "Can't remove job from queue");
			if (0 != (retval = pthread_mutex_unlock(&tp->queue->mutex)))
				fprintf(stderr, "Mutex unlock error\n");
			return -1;
		}
	
		if (0 != (retval = pthread_mutex_unlock(&tp->queue->mutex))) {
			fprintf(stderr, "Mutex unlock error\n");
			return retval;
		}

		task_func_t dojob = *routine;
		dojob(*args);	
		free(job);	
	}
	return 0;
}

void* worker_thread_routine(void* args) {
	thread_pool* tp = (thread_pool*) args;
	if (NULL == tp)
		pthread_exit(NULL);
	while (1 == tp->alive) {
		task_func_t proc;
		void* proc_args;
		if (0 != __tp_get_and_done(tp, &proc, &proc_args)) {
			fprintf(stderr, "worker_thread_routine()\n");
			pthread_exit(NULL);
		}
	}
	pthread_exit(NULL);
}

int tp_add_task(thread_pool* tp, task_func_t routine, void* args) {
	if (NULL == tp) {
		fprintf(stderr, "Null threadpool argument");
		return -1;
	}
	tp_node* newJob = (tp_node*)malloc(sizeof(tp_node));
	if (NULL == newJob) {
		fprintf(stderr, "job allocation memory error\n");
		return -1;
	}
	newJob->function = routine;
	newJob->args = args;
	
	int retval;
	if (0 != (retval = pthread_mutex_lock(&tp->queue->mutex))) {
		fprintf(stderr, "can't lock mutex\n");
		return retval;	
	}
	tp_queue_add(tp, newJob);
	if (0 != (retval = pthread_mutex_unlock(&tp->queue->mutex))) {
		fprintf(stderr, "can't unlock mutex\n");
		return retval;
	}
	
	return 0;
}	

int tp_destroy(thread_pool* tp) {
	if (NULL == tp) {
		fprintf(stderr, "Null threadpool argument");
		return -1;
	}
	if (0 == tp->alive) {
		fprintf(stderr, "Threadpool is already dead\n");
		return -1;
	}
	tp->alive = 0;
	
	int i;
	int err;
	for (i = 0; i < tp->threads_cnt; ++i) {
		if (0 != sem_post(tp->queue->queueSem)) {
			perror("sem_post\n");
			return errno;
		}
	}
	
	if (0 != sem_destroy(tp->queue->queueSem)) {
		perror("sem_destroy");
		return errno;
	}
	int retval;
	for (i = 0; i < tp->threads_cnt; ++i) {
		if (0 != (retval = pthread_join(tp->threads[i], NULL))) {
			fprintf(stderr, "joining thread [%d] error\n", i);
			return retval;
		}
	}

	tp_queue_remove_queue(tp);
	free(tp->threads);
	free(tp->queue);
	
	return 0;
}

int tp_join(thread_pool* tp) {
	if (NULL == tp) {
		fprintf(stderr, "Null threadpool argument");
		return -1;
	}
	while (0 != tp->queue->jobs_cnt) { };

	return 0;
}


int tp_queue_init(thread_pool* tp) {
	tp->queue = (tp_queue*)malloc(sizeof(tp_queue));
	if (NULL == tp->queue)
		return -1;
	tp->queue->tail = NULL;
	tp->queue->head = NULL;
	tp->queue->jobs_cnt = 0;

	int retval;
	if (0 != (retval = pthread_mutex_init(&tp->queue->mutex, NULL))) {
		fprintf(stderr, "can't initialize mutex\n");
		return retval;
	}

	tp->queue->queueSem = (sem_t*)malloc(sizeof(sem_t));
	if (NULL == tp->queue->queueSem) {
		free(tp->queue);
		return -1;
	}
	if (0 != sem_init(tp->queue->queueSem, 0, 0)) {
		free(tp->queue->queueSem);
		free(tp->queue);
		return -1;
	}
	return 0;
}

void tp_queue_add(thread_pool* tp, tp_node* newJob) {
	newJob->next = NULL;
	newJob->prev = NULL;

	tp_node* oldFirstJob;
	oldFirstJob = tp->queue->head;
	
	switch(tp->queue->jobs_cnt) {
		case 0:
			tp->queue->tail = newJob;
			tp->queue->head = newJob;
			break;
		default:
			oldFirstJob->prev = newJob;
			newJob->next = oldFirstJob;
			tp->queue->head = newJob;
	}

	tp->queue->jobs_cnt += 1;
	sem_post(tp->queue->queueSem);
}

int tp_queue_remove(thread_pool* tp) {
	if (NULL == tp)
		return -1;
	tp_node* oldLastJob;
	oldLastJob = tp->queue->tail;

	switch(tp->queue->jobs_cnt) {
		case 0:
			return -1;
			break;
		case 1:
			tp->queue->tail = NULL;
			tp->queue->head = NULL;
			break;
		default:
			oldLastJob->prev->next = NULL;
			tp->queue->tail = oldLastJob->prev;
	}
	tp->queue->jobs_cnt -= 1;
	return 0;
}


tp_node* tp_queue_get(thread_pool* tp) {
	if (NULL == tp)
		return NULL;
	return tp->queue->tail;
}

void tp_queue_remove_queue(thread_pool* tp) {
	tp_node* curJob;
	curJob = tp->queue->tail;
	
	while (0 != tp->queue->jobs_cnt) {
		tp->queue->tail = curJob->prev;
		free(curJob);
		curJob = tp->queue->tail;
		tp->queue->jobs_cnt -= 1;
	}
	
	tp->queue->head = NULL;
	tp->queue->tail = NULL;
	
	if (0 != sem_destroy(tp->queue->queueSem)) {
		fprintf(stderr, "can't destroy semaphore");
	}
	free(tp->queue->queueSem);

	if (0 != pthread_mutex_destroy(&tp->queue->mutex)) {
		fprintf(stderr, "can't destroy mutex");
	}
}
