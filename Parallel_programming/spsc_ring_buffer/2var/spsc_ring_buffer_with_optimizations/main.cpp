#include <iostream>
#include <future>
#include <thread>
#include <vector>

#include "spsc_ring_buffer_abstract.h"
#include "spsc_ring_buffer.h"
#include "spsc_ring_buffer_opt1.h"
#include "spsc_ring_buffer_opt2.h"
#include "thread_safe_queue.h"


#define CAPACITY 1024
#define TEST_NUMBERS_CNT 10000000

long consumer_work_loop(spsc_ring_buffer_abstract<int>& channel) {
    long sum = 0;
    for (int i = 1; i < TEST_NUMBERS_CNT; i++) {
        int elem;
        while (!channel.dequeue(elem)) {
            std::this_thread::yield();
        }
        sum += elem;
    }

    return sum;
}

long producer_work_loop(spsc_ring_buffer_abstract<int>& channel) {
    long sum = 0;
    for (int i = 1; i < TEST_NUMBERS_CNT; i++) {
        while (!channel.enqueue(i)) {
            std::this_thread::yield();
        }
        sum += i;
    }
    return sum;
}

int main() {
    std::clock_t start;
    start = std::clock();
    thread_safe_queue<int> channel_q;
    std::future<long> produced_result = async(std::launch::async, producer_work_loop, std::ref(channel_q));
    std::future<long> consumed_result = async(std::launch::async, consumer_work_loop, std::ref(channel_q));
    channel_q.shutdown();
    if (0 == (produced_result.get() - consumed_result.get())) {
        std::cout << "thread_safe_queue:       " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000)
                << " ms" << std::endl;
    } else {
        std::cout << "Incorrect: thread_safe_queue" << std::endl;
    }

    start = std::clock();
    spsc_ring_buffer<int> channel(CAPACITY);
    produced_result = async(std::launch::async, producer_work_loop, std::ref(channel));
    consumed_result = async(std::launch::async, consumer_work_loop, std::ref(channel));
    if (0 == (produced_result.get() - consumed_result.get())) {
        std::cout << "spsc_ring_buffer:        " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000)
                << " ms" << std::endl;
    } else {
        std::cout << "Incorrect: spsc_ring_buffer" << std::endl;
    }

    start = std::clock();
    spsc_ring_buffer_opt1<int> channel_1(CAPACITY);
    produced_result = async(std::launch::async, producer_work_loop, std::ref(channel_1));
    consumed_result = async(std::launch::async, consumer_work_loop, std::ref(channel_1));
    if (0 == (produced_result.get() - consumed_result.get())) {
        std::cout << "spsc_ring_buffer_opt1:   " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000)
                << " ms" << std::endl;
    } else {
        std::cout << "Incorrect: spsc_ring_buffer_opt1" << std::endl;
    }

    start = std::clock();
    spsc_ring_buffer_opt2<int> channel_2(CAPACITY);
    produced_result = async(std::launch::async, producer_work_loop, std::ref(channel_2));
    consumed_result = async(std::launch::async, consumer_work_loop, std::ref(channel_2));
    if (0 == (produced_result.get() - consumed_result.get())) {
        std::cout << "spsc_ring_buffer_opt2:   " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000)
                << " ms" << std::endl;
    } else {
        std::cout << "Incorrect: spsc_ring_buffer_opt2" << std::endl;
    }

    return 0;
}