/*
 * g++ -std=c++11 -pthread -I /home/sonya/Downloads/boost_1_57_0 main.cpp -L /home/sonya/Downloads/boost_1_57_0/libs/ -lboost_thread -lboost_system -o a
 */

#include <iostream>
#include <forward_list>
#include <boost/thread.hpp>
#include "striped_hash_set.h"

#define THREADS_CNT 2


int main(){
    striped_hash_set<int> hash_table(16);
    std::vector<std::thread> threads;

    for (int i = 0; i < THREADS_CNT; i++) {
        threads.emplace_back([&](){
            for (int j = 0; j < 40; j++) {
                std::random_device generator;
                std::uniform_int_distribution<int> distribution(1, 100);
                int k = distribution(generator);
                hash_table.add(k);
            }
            for (int j = 0; j < 30; j++) {
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