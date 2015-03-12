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

using namespace std;

template <typename R>
class thread_pool  {
    thread_safe_queue<pair<function<R()>, promise<R> > > tasks;
    vector<auto_thread_guard> executant_threads;

public :
    thread_pool(int size = 4) {
        for (int i = 0; i < size; ++i) {
            executant_threads.emplace_back(thread([&](){
                pair< function <R()>, promise<R> > task;
                while (tasks.pop(task)) {
                    R result = task.first();
                    try {
                        task.second.set_value(result);
                    } catch (exception&) {
                        task.second.set_exception(current_exception());
                    }
                }
            }));
        }
    }
    ~thread_pool() {
        tasks.shutdown();
        /*
        for (int i = 0; i < executant_threads.size(); ++i) {
            if (executant_threads[i].joinable()) {
                executant_threads[i].join();
            }
        }
        */
    }
    future<R> execute(function<R()> func_with_args) {
        promise<R> task;
        future<R> res = task.get_future();
        tasks.push(make_pair(func_with_args, move(task)));
        return res;
    }
};

#endif //THREADPOOL_H