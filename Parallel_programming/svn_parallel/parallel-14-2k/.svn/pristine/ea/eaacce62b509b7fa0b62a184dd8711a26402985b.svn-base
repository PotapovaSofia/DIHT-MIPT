#ifndef BARRIER_H_INCLUDED
#define BARRIER_H_INCLUDED

#include <iostream>
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <pthread.h>
#include <thread>
#include <vector>

using namespace std;

class cyclic_barrier {
    condition_variable barrier_not_open_cond;
    mutex mtx;
    int size;
    atomic_int reverse_counter;
    atomic_int counter;

public:
    cyclic_barrier(int size_ = 10): size(size_), counter(0), reverse_counter(0) {}
    ~cyclic_barrier() {}
    void enter() {
        unique_lock<mutex> lock(mtx);
        ++counter;
        if (counter != size) {
            while (0 == reverse_counter)
                barrier_not_open_cond.wait(lock);
            --reverse_counter;
            if (0 != reverse_counter)
                barrier_not_open_cond.notify_one();
        } else {
            counter = 0;
            reverse_counter = size - 1;
            barrier_not_open_cond.notify_one();
        }
    }
};

#endif // BARRIER_H_INCLUDED