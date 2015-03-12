#ifndef THREAD_GUARD_FOR_POOL_H
#define THREAD_GUARD_FOR_POOL_H

#include <mutex>
#include <thread>
#include <iostream>

using namespace std;

class auto_thread_guard {
    thread tthread;
public:
    auto_thread_guard(thread thread_): tthread(move(thread_)) {}
    ~auto_thread_guard() {
        if (tthread.joinable())
            tthread.join();
    }
    auto_thread_guard(auto_thread_guard&& other) {
        tthread = move(other.tthread);
    }
};

#endif // THREAD_GUARD_FOR_POOL_H