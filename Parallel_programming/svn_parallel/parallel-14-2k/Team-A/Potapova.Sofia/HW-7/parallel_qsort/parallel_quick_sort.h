#ifndef OLD_QSORT_PARALLEL_QUICK_SORT_H
#define OLD_QSORT_PARALLEL_QUICK_SORT_H

#include <vector>
#include <future>
#include <algorithm>
#include "thread_pool.h"

#define iterator std::vector<int>::iterator

const int depth_limit = 20;

template <typename T>
class sort_task {
public:
    thread_pool<void>& task_queue;
    std::vector<T>& array;
    iterator begin;
    iterator end;
    int depth;

    sort_task(std::vector<T>& array_, const iterator& begin_, const iterator& end_,
              thread_pool<void>& task_queue_, int depth_):
            array(array_), begin(begin_), end(end_), task_queue(task_queue_), depth(depth_) {}

    void operator()() {

        auto size = std::distance(begin, end);
        std::cout << depth << std::endl;
        if (depth > depth_limit) {
            std::sort(begin, end);
            return;
        }

        auto it = end - 1;
        std::iter_swap(begin + size / 2, it);
        auto pivot_it = std::partition(begin, it, [it](T v) { return v < *it; });
        std::iter_swap(pivot_it, it);

        sort_task sort_left_part(array, begin, pivot_it, task_queue, depth + 1);
        sort_task sort_right_part(array, pivot_it + 1, end, task_queue, depth + 1);

        auto left_part_sorted = task_queue.submit(sort_left_part);
        sort_right_part();

        task_queue.active_wait(left_part_sorted);
    }
};

template <typename T>
void parallel_quick_sort(std::vector<T>& data) {
    thread_pool<void> task_queue;
    sort_task<T> sort_whole(data, data.begin(), data.end(), task_queue, 0);
    auto res = task_queue.submit(sort_whole);
    res.get();
}

#endif //OLD_QSORT_PARALLEL_QUICK_SORT_H
