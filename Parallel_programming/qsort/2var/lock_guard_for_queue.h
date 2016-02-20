#ifndef LOCK_GUARD_FOR_QUEUE_H
#define LOCK_GUARD_FOR_QUEUE_H

#include <mutex>
#include <thread>
#include <iostream>

class auto_lock_guard {
    std::mutex& mtx;

public:
    auto_lock_guard(std::mutex& mtx_): mtx(mtx_) {
        mtx.lock();
    }

    ~auto_lock_guard() {
        mtx.unlock();
    }
};

#endif // LOCK_GUARD_FOR_QUEUE_H