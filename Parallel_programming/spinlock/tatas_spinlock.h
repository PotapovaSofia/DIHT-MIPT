#ifndef _SPINLOCK_TATAS_SPINLOCK_H_
#define _SPINLOCK_TATAS_SPINLOCK_H_

#include <thread>
#include <atomic>

class spinlock_tatas: public spinlock_t {

    std::atomic<bool> locked;

public:

    spinlock_tatas(): locked(false) {}
    ~spinlock_tatas() {}

    void lock() {
        while (true) {
            while (locked.load(std::memory_order_acquire)) {
                std::this_thread::yield();
            }
            if (locked.exchange(true, std::memory_order_acquire)) {
                std::this_thread::yield();
            } else {
                break;
            }
        }
    }

    bool try_lock() {
        return !locked.exchange(true, std::memory_order_acquire);
    }

    void unlock() {
        locked.store(false, std::memory_order_release);
    }
};

#endif //_SPINLOCK_TATAS_SPINLOCK_H_
