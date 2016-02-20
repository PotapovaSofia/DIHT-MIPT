#include <iostream>
#include <vector>
#include <thread>
#include <random>
#include "lock_free_stack.h"

#define THREADS_CNT 4

int main() {
    lock_free_stack<int> stack;
    std::vector<std::thread> threads;
    for(int i = 0; i < THREADS_CNT; i++) {
        threads.emplace_back([&]{
            for (int j = 0; j < 1000; j++) {
                std::random_device generator;
                std::uniform_int_distribution<int> distribution(1, 100);
                int k = distribution(generator);
                stack.push(k);
                std::shared_ptr<int> c = stack.pop();
                if(c) {
                    std:: cout << std::this_thread::get_id() << ": " << *c << std::endl;
                } else {
                    std:: cout << std::this_thread::get_id() << ": -" << std::endl;
                }
            }
        });
    }
    for (auto& th: threads) {
        th.join();
    }
    return 0;
}