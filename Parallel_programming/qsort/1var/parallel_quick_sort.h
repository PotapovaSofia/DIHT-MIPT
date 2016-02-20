#ifndef OLD_QSORT_PARALLEL_QUICK_SORT_H
#define OLD_QSORT_PARALLEL_QUICK_SORT_H

#include <vector>
#include <future>
#include <algorithm>

#define SWITCH_LIMIT 40
#define iterator std::vector<int>::iterator

class sort_task {
public:
    thread_pool<void>& task_queue;
    std::vector<int>& array;
    iterator begin;
    iterator end;

    sort_task(std::vector<int>& array_, const iterator& begin_, const iterator& end_,
              thread_pool<void>& task_queue_):
            array(array_), begin(begin_), end(end_), task_queue(task_queue_) {}

    void operator()() {

        auto size = std::distance(begin, end);
        if (size <= SWITCH_LIMIT) {
            std::sort(begin, end);
            return;
        }

        auto it = end - 1;
        std::iter_swap(begin + size / 2, it);
        auto pivot_it = std::partition(begin, it, [it](int v) { return v < *it; });
        std::iter_swap(pivot_it, it);

        sort_task sort_left_part(array, begin, pivot_it, task_queue);
        sort_task sort_right_part(array, pivot_it + 1, end, task_queue);

        auto left_part_sorted = task_queue.submit(sort_left_part);
        sort_right_part();

        task_queue.active_wait(left_part_sorted);
    }
};

void parallel_quick_sort(std::vector<int>& data) {
    thread_pool<void> task_queue;
    sort_task sort_whole(data, data.begin(), data.end(), task_queue);
    task_queue.submit(sort_whole);
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

#endif //OLD_QSORT_PARALLEL_QUICK_SORT_H
