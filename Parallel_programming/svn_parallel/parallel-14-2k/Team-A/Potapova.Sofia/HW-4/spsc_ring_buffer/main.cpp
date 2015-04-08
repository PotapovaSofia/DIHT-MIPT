#include <iostream>
#include <future>
#include <thread>

#include "spsc_ring_buffer.h"

#define CAPACITY 1024
#define TEST_NUMBERS_CNT 10000000

long consumer_work_loop(spsc_ring_buffer<int>& channel) {
    long sum = 0;
    for (int i = 1; i < TEST_NUMBERS_CNT; i++) {
        int elem;
        while (!channel.dequeue(elem)) {
            std::this_thread::yield();
        }
        //std::cout << std::this_thread::get_id() << ": d: " << extracted_element << std::endl;
        sum += elem;
    }

    return sum;
}

long producer_work_loop(spsc_ring_buffer<int>& channel) {
    long sum = 0;
    for (int i = 1; i < TEST_NUMBERS_CNT; i++) {
        while (!channel.enqueue(i)) {
            std::this_thread::yield();
        }
        //std::cout << std::this_thread::get_id() << ": e: " << i << std::endl;
        sum += i;
    }
    return sum;
}

int main() {
    spsc_ring_buffer<int> channel(CAPACITY);
    std::future<long> produced_result = async(std::launch::async, producer_work_loop, std::ref(channel));
    std::future<long> consumed_result = async(std::launch::async, consumer_work_loop, std::ref(channel));
    if (0 == (produced_result.get() - consumed_result.get())) {
        std::cout << "Correct" << std::endl;
    } else {
        std::cout << "Incorrect" << std::endl;
    }

    return 0;
}