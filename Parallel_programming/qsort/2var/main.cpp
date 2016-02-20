#include <iostream>
#include <list>
#include <future>
#include <vector>
#include "thread_pool.h"
#include <algorithm>

#define SWITCH_LIMIT 2

class sort_task {
public:
    thread_pool<void>& task_queue;
    std::vector<int>& array;
    size_t begin;
    size_t end;

    sort_task(std::vector<int>& array_, size_t begin_, size_t end_, thread_pool<void>& task_queue_):
            array(array_), begin(begin_), end(end_), task_queue(task_queue_) {}

    void operator()() {
        if(end - begin <= SWITCH_LIMIT) {
            std::sort(array.begin() + begin, array.begin() + end + 1);
            return;
        }
        int pivot = array[(end + begin)/2];
        int i = begin, j = end;
        do {
            while(array[i] < pivot) i++;
            while(array[j] > pivot) j--;
            if(i <= j) std::swap(array[i++], array[j--]);
        } while(i <= j);

        sort_task sort_left_part(array, begin, j, task_queue);
        sort_task sort_right_part(array, i, end, task_queue);

        auto left_part_sorted = task_queue.submit(sort_left_part);
        sort_right_part();

        task_queue.active_wait(std::move(left_part_sorted));
        //task_queue.active_wait(left_part_sorted);
    }
};

int main() {
    thread_pool<void> task_queue;

    std::vector<int> data;
    for (int i = 0; i < 10000; i++) {
        std::random_device generator;
        std::uniform_int_distribution<int> distribution(1, 100);
        data.push_back(distribution(generator));
    }
    sort_task sort_whole(data, 0, data.size() - 1, task_queue);
    task_queue.submit(sort_whole);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    if(std::is_sorted(sort_whole.array.begin(), sort_whole.array.end())) {
        std::cout << "Correct" << std::endl;
    } else {
        std::cout << "Incorrect" << std::endl;
    }
    return 0;
}