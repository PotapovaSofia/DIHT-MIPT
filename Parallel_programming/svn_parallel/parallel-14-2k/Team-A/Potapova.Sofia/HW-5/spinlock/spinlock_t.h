#ifndef SPINLOCK_T_H_
#define SPINLOCK_T_H_

#include <thread>
#include <atomic>

class spinlock_t {
public:
    spinlock_t() {}
    ~spinlock_t() {}

    virtual void lock() = 0;
    virtual bool try_lock() = 0;
    virtual void unlock() = 0;
};
#endif //SPINLOCK_T_H_
