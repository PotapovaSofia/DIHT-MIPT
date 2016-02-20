#ifndef THREAD_SAFE_QUEUE_H
#define THREAD_SAFE_QUEUE_H

#include <iostream>
#include <atomic>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <queue>

template <typename T>
class thread_safe_queue {
    std::queue<T> ordinary_queue;
    std::mutex queue_mtx;
    std::condition_variable queue_not_empty_cond;
    bool is_final;

public:
    thread_safe_queue(): is_final(false) {}

    ~thread_safe_queue() {}

    bool pop(T& answer) {
        std::unique_lock<std::mutex> lock(queue_mtx);
        while (ordinary_queue.empty() && !is_final) {
            queue_not_empty_cond.wait(lock);
        }
        if (!ordinary_queue.empty()) {
            answer = move(ordinary_queue.front());
            ordinary_queue.pop();
            return true;
        }
        return false;
    }

    bool try_pop(T& answer) {
        std::lock_guard<std::mutex> lock(queue_mtx);
        if (ordinary_queue.empty()) {
            return false;
        }
        answer = std::move(ordinary_queue.front());
        ordinary_queue.pop();
        return true;
    }


    void push(T element) {
        std::lock_guard<std::mutex> lock(queue_mtx);
        if(!is_final) {
            ordinary_queue.push(move(element));
            queue_not_empty_cond.notify_one();
        }
    }

    void shutdown() {
        std::lock_guard<std::mutex> lock(queue_mtx);
        is_final = true;
        queue_not_empty_cond.notify_all();
    }
};

#endif //THREAD_SAFE_QUEUE_H