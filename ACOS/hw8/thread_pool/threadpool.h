#ifndef THREAD_POOL
#define THREAD_POOL

#include <pthread.h>
#include <semaphore.h>

typedef void (*task_func_t) (void*);

/*Job*/
typedef struct tp_node {
        task_func_t function;
        void* args;             // function arguements
        struct tp_node* next;
        struct tp_node* prev;
} tp_node;

/* Queue of jobs*/
typedef struct tp_queue {
        tp_node* head;
        tp_node* tail;
        int jobs_cnt;
        sem_t* queueSem;        // need to wait in case of an empty queue
        pthread_mutex_t mutex;  // control access to the queue
} tp_queue;
   
/* Threadpool */
typedef struct thread_pool {
	int alive;		
	pthread_t* threads;	
	int threads_cnt;		
	tp_queue* queue;	
} thread_pool;

/*Threadpool methods*/
int tp_init(thread_pool* tp, int cnt);	
int tp_add_task(thread_pool* tp, task_func_t routine, void* args);
int tp_join(thread_pool* tp);					
int tp_destroy(thread_pool* tp);				
void* worker_thread_routine(void* args);	
int __tp_get_and_done(thread_pool* tp, task_func_t* routine, void** args);

/*Queue methods*/
int tp_queue_init(thread_pool* tp);                        
void tp_queue_add(thread_pool* tp, tp_node* newjob);      
tp_node* tp_queue_get(thread_pool* tp);           
int tp_queue_remove(thread_pool* tp);              
void tp_queue_remove_queue(thread_pool* tp);  
#endif
