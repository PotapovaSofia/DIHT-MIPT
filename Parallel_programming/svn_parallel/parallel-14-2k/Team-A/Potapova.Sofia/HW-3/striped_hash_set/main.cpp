#include <iostream>
#include <forward_list>
#include <boost/thread.hpp>
#include <thread>
#include "striped_hash_set.h"

#define THREADS_CNT 4

int main(){
    striped_hash_set<int> hash_table(16);
    std::vector<std::thread> threads;

    for (int i = 0; i < THREADS_CNT; i++) {
        threads.emplace_back([&](){
            for (int j = 0; j < 5; j++) {
                std::random_device generator;
                std::uniform_int_distribution<int> distribution(1, 100);
                int k = distribution(generator);
                hash_table.add(k);
            }
            for (int j = 0; j <5; j++) {
                std::random_device generator;
                std::uniform_int_distribution<int> distribution(1, 100);
                int k = distribution(generator);
                if (hash_table.contains(k)) {
                    hash_table.remove(k);
                }
            }
        });
    }

    for (auto& th: threads) {
        th.join();
    }

    return 0;
}