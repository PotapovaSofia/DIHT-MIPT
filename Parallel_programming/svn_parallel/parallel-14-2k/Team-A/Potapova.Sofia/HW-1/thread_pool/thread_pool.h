#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <iostream>
#include <atomic>
#include <functional>
#include <thread>
#include <vector>
#include <queue>
#include <future>

#include "thread_guard_for_pool.h"
#include "thread_safe_queue.h"

template <typename R>
class thread_pool  {
    thread_safe_queue<std::pair<std::function<R()>, std::promise<R>>> tasks;
    std::vector<std::thread> workers;

public :
    thread_pool() {
        execute_workers(get_default_num_workers());
    }
    thread_pool(int thread_cnt) {
        execute_workers(thread_cnt);
    }

    ~thread_pool() {
        tasks.shutdown();
        for (int i = 0; i < workers.size(); ++i) {
              if (workers[i].joinable()) {
                  workers[i].join();
              }
        }

    }

    std::future<R> submit(std::function<R()> task) {
        std::promise<R> prom;
        std::future<R> res = prom.get_future();
        tasks.push(make_pair(task, move(prom)));
        return res;
    }

private:
    int get_default_num_workers() {
        int thread_cnt = std::thread::hardware_concurrency();
        if (thread_cnt == 0) {
            thread_cnt = 4;
        }
        return thread_cnt;
    }

    void execute_workers(int size) {
        for (int i = 0; i < size; ++i) {
            workers.emplace_back(&thread_pool::worker_fn, this);
        }
    }

    void worker_fn() {
        std::pair<std::function <R()>, std::promise<R>> task;
        while (tasks.pop(task)) {
            try {
                R result = task.first();
                task.second.set_value(result);
            } catch (...) {
                task.second.set_exception(std::current_exception());
            }
        }
    }
};

#endif //THREADPOOL_H