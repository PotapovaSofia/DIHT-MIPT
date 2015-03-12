#ifndef LOCK_GUARD_FOR_QUEUE_H
#define LOCK_GUARD_FOR_QUEUE_H

#include <mutex>
#include <thread>
#include <iostream>

using namespace std;

class auto_lock_guard {
    mutex& mtx;
public:
    auto_lock_guard(mutex& mtx_): mtx(mtx_) {
        mtx.lock();
    }
    ~auto_lock_guard() {
        mtx.unlock();
    }
};

#endif // LOCK_GUARD_FOR_QUEUE_H