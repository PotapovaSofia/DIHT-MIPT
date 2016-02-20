#ifndef BARRIER_H_INCLUDED
#define BARRIER_H_INCLUDED

#include <iostream>
#include <atomic>
#include <thread>
#include <vector>

class cyclic_barrier {
    bool is_barrier_open;
    std::mutex mtx;
    int size;
    int counter;
    bool allow;
    std::condition_variable come_to_barrier_cond;

public:
    cyclic_barrier(int size_ = 10): size(size_), counter(0), allow(true) {}
    ~cyclic_barrier() {}
    void enter() {
        std::unique_lock<std::mutex> lock(mtx);
        while (!allow) {
            come_to_barrier_cond.wait(lock);
        }
        ++counter;
        if (counter == size){
            allow = false;
        }

        while (counter != size && allow) {
            barrier_not_open_cond.wait(lock);
        }
        if (counter == size) {
            counter--;
            barrier_not_open_cond.notify_all();
        } else {
            counter--;
        }
        if (counter == 0) {
            allow = true;
            come_to_barrier_cond.notify_all();
        }
    }
};


#endif // BARRIER_H_INCLUDED