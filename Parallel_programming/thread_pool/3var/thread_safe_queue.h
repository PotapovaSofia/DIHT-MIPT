#ifndef THREAD_SAFE_QUEUE_H
#define THREAD_SAFE_QUEUE_H

#include <iostream>
#include <atomic>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <queue>

#include "lock_guard_for_queue.h"

using namespace std;

template <typename T>
class thread_safe_queue {
    queue<T> queue_for_tasks;
    mutex queue_mtx;
    condition_variable queue_not_empty_cond;
    bool is_final;

public:
    thread_safe_queue(): is_final(false) {}
    ~thread_safe_queue() {}
    bool pop(T& answer) {
        unique_lock<mutex> lock(queue_mtx);
        while (queue_for_tasks.empty() && !is_final) {
            queue_not_empty_cond.wait(lock);
        }
        if (!queue_for_tasks.empty()) {
            answer = move(queue_for_tasks.front());
            queue_for_tasks.pop();
            return true;
        }
        return false;
    }
    void push(T element) {
        auto_lock_guard lock(queue_mtx);
        queue_for_tasks.push(move(element));
        queue_not_empty_cond.notify_one();
    }
    void shutdown() {
        unique_lock<mutex> lock(queue_mtx);
        is_final = true;
        queue_not_empty_cond.notify_all();
    }
};

#endif //THREAD_SAFE_QUEUE_H