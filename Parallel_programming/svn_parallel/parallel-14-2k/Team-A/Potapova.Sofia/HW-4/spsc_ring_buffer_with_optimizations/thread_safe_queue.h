#ifndef _THREAD_SAFE_QUEUE_H_
#define _THREAD_SAFE_QUEUE_H_

#include <iostream>
#include <atomic>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <queue>
#include "spsc_ring_buffer_abstract.h"

template <typename T>
class thread_safe_queue: public spsc_ring_buffer_abstract<T> {
    std::queue<T> ordinary_queue;
    std::mutex queue_mtx;
    std::condition_variable queue_not_empty_cond;
    bool is_final;

public:
    thread_safe_queue(): is_final(false) {}

    ~thread_safe_queue() {}

    bool dequeue(T& answer) {
        std::unique_lock<std::mutex> lock(queue_mtx);
        while (ordinary_queue.empty() && !is_final) {
            queue_not_empty_cond.wait(lock);
        }
        if (!ordinary_queue.empty()) {
            answer = std::move(ordinary_queue.front());
            ordinary_queue.pop();
            return true;
        }
        return false;
    }

    bool enqueue(T element) {
        std::lock_guard<std::mutex> lock(queue_mtx);
        ordinary_queue.push(std::move(element));
        queue_not_empty_cond.notify_one();
        return true;
    }

    void shutdown() {
        std::unique_lock<std::mutex> lock(queue_mtx);
        is_final = true;
        queue_not_empty_cond.notify_all();
    }
};

#endif //_THREAD_SAFE_QUEUE_H_
