#ifndef THREAD_GUARD_FOR_POOL_H
#define THREAD_GUARD_FOR_POOL_H

#include <mutex>
#include <thread>
#include <iostream>

class auto_thread_guard {
    std::thread tthread;

public:
    auto_thread_guard(std::thread thread_): tthread(move(thread_)) {}
    auto_thread_guard(std::function<void()> fn): tthread(fn) {}

    ~auto_thread_guard() {
        if (tthread.joinable())
            tthread.join();
    }

    auto_thread_guard(auto_thread_guard&& other) {
        tthread = move(other.tthread);
    }
};

#endif // THREAD_GUARD_FOR_POOL_H