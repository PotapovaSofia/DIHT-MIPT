#include <iostream>
#include <chrono>
#include <vector>
#include "tas_spinlock.h"
#include "tatas_spinlock.h"

#define ITER_CNT 10000000

void worker_fn(spinlock_t& lock, int& k) {
    for (size_t i = 0; i < ITER_CNT; ++i) {
        lock.lock();
        k++;
        lock.unlock();
    }
}

int main() {
    for (int N = 1; N <= 10; ++N) {
        std::vector<std::thread> threads;

        spinlock_tas lock_t;
        int k = 0;
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < N; ++i) {
            threads.emplace_back(worker_fn, std::ref(lock_t), std::ref(k));
        }
        for (auto& t : threads) {
            t.join();
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = end - start;
        std::cout << "TAS spinlock:   " << N << " threas: " << diff.count() << " seconds" << std::endl;

        threads.clear();
        spinlock_tatas lock_tt;
        k = 0;
        start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < N; ++i) {
            threads.emplace_back(worker_fn, std::ref(lock_tt), std::ref(k));
        }
        for (auto& t : threads) {
            t.join();
        }
        end = std::chrono::high_resolution_clock::now();
        diff = end - start;
        std::cout << "TATAS spinlock: " << N << " threas: " << diff.count() << " seconds" << std::endl;
    }

    return 0;
}