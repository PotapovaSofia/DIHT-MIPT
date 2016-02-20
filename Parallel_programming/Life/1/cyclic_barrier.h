#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <assert.h>

using namespace std;


class cyclic_barrier {
    pthread_cond_t barrier_not_open_cond;
    pthread_mutex_t mtx;
    size_t num_threads;
    size_t entered_count;
    bool allow;
    pthread_cond_t come_to_barrier_cond;

public:
    cyclic_barrier() {
    	num_threads = 4;
    	entered_count = 0;
    	allow = true;
    	/*
    	barrier_not_open_cond = PTHREAD_COND_INITIALIZER;
    	come_to_barrier_cond = PTHREAD_COND_INITIALIZER;
    	mtx = PTHREAD_MUTEX_INITIALIZER;
    	*/
    	pthread_cond_init(&barrier_not_open_cond, NULL);
    	pthread_cond_init(&come_to_barrier_cond, NULL);
        //barrier_not_open_cond = PTHREAD_COND_INITIALIZER;
        //come_to_barrier_cond = PTHREAD_COND_INITIALIZER;
    	pthread_mutex_init(&mtx, NULL);
        //mtx = PTHREAD_MUTEX_INITIALIZER;
    }
    ~cyclic_barrier() {
    	//assert (pthread_cond_destroy(&barrier_not_open_cond));
        pthread_cond_destroy(&barrier_not_open_cond);
    	//assert (pthread_cond_destroy(&come_to_barrier_cond));
        pthread_cond_destroy(&come_to_barrier_cond);
    	//assert (pthread_mutex_destroy(&mtx));
        pthread_mutex_destroy(&mtx);
    }
    void set_size(int thread_cnt) {
    	num_threads = thread_cnt;
    }
    void enter() {
		//assert (pthread_mutex_lock(&mtx));
        pthread_mutex_lock(&mtx);
        while (!allow) {
        	//assert (pthread_cond_wait(&come_to_barrier_cond, &mtx));
            pthread_cond_wait(&come_to_barrier_cond, &mtx);
        }
        ++entered_count;
        if (entered_count == num_threads){
            allow = false;
        }

        while (entered_count != num_threads && allow) {
            //assert (pthread_cond_wait(&barrier_not_open_cond, &mtx));
            pthread_cond_wait(&barrier_not_open_cond, &mtx);
        }
        if (entered_count == num_threads) {
            entered_count--;
            //assert (pthread_cond_broadcast(&barrier_not_open_cond));
            pthread_cond_broadcast(&barrier_not_open_cond);
        } else {
            entered_count--;
        }
        if (entered_count == 0) {
            allow = true;
            //assert (pthread_cond_broadcast(&come_to_barrier_cond));
            pthread_cond_broadcast(&come_to_barrier_cond);
        }
        //assert (pthread_mutex_unlock(&mtx));
        pthread_mutex_unlock(&mtx);
    }
};
