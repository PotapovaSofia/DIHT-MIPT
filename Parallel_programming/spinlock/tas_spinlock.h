#ifndef TAS_SPINLOCK_H_
#define TAS_SPINLOCK_H_

#include <thread>
#include <atomic>
#include "spinlock_t.h"

class spinlock_tas: public spinlock_t {

    std::atomic<bool> locked;

public:

    spinlock_tas(): locked(false) {}
    ~spinlock_tas() {}

    void lock() {
        while (locked.exchange(true, std::memory_order_acquire)) {
            std::this_thread::yield();
        }
    }

    bool try_lock() {
        return !locked.exchange(true, std::memory_order_acquire);
    }

    void unlock() {
        locked.store(false, std::memory_order_release);
    }
};

#endif //TAS_SPINLOCK_H_
