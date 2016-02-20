#ifndef GAUSS_PARALLEL_GAUSS_H
#define GAUSS_PARALLEL_GAUSS_H

#include <vector>
#include <thread>
#include <algorithm>

#include "cyclic_barrier.h"
#include "atomic_wrapper.h"


cyclic_barrier barrier;
cyclic_barrier main_barrier;

void worker_fn(int);

bool parallel_gauss(int n, std::vector<std::vector<double>> a, std::vector<double>& x) {
    bool flag(true);
    int thread_cnt = std::thread::hardware_concurrency();
    if (thread_cnt == 0) {
        thread_cnt = 4;
    }
    barrier.set_size(thread_cnt);
    main_barrier.set_size(thread_cnt+1);
    std::vector<std::pair<double, int>> max(thread_cnt, std::make_pair(0.0, 0));
    std::vector<atomwrapper<bool>> ready;
    for (int i = 0; i < n; i++) {
        ready.emplace_back(false);
    }
    std::vector<std::thread> threads;
    for (int thread_id = 0; thread_id < thread_cnt; thread_id++) {
        threads.emplace_back(worker_fn, thread_id, n, thread_cnt, a,
                             ready, max, flag, x);
    }
    main_barrier.enter();
    for(auto& th: threads) {
        th.join();
    }
    return flag;
}

void worker_fn(int thread_id, int n, int thread_cnt, std::vector<std::vector<double>>& a,
               std::vector<atomwrapper<bool>>& ready,
               std::vector<std::pair<double, int>>& max, bool& flag, std::vector<double>& x) {
    for (int p = 0; p < n-1; p++) {
        for (int i = thread_id + p; i < n; i+=thread_cnt) {
            if (abs(a[i][p]) > max[thread_id].first) {
                max[thread_id].first = abs(a[i][p]);
                max[thread_id].second = i;
            }
        }
        barrier.enter();
        if(thread_id == 0) {
            auto it_max = std::max_element(max.begin(), max.end());
            if (it_max->first == 0) {
                flag = false;
                return;
            }
            std::swap(a[p], a[it_max->second]);
        }
        barrier.enter();
        for (int j = thread_id + p + 1; j < n; j +=thread_cnt) {
            double coef = a[j][p] / a[p][p];
            for (int k = p; k <=n; k++) {
                a[j][k] -= coef * a[p][k];
            }
        }
        barrier.enter();
        if (thread_id == 0) {
            max.assign(thread_cnt, std::make_pair(0.0, 0));
        }
        barrier.enter();
    }

    for (int i = n - 1 - thread_id; i >= 0; i-=thread_cnt) {
        x[i] = a[i][n] / a[i][i];
        while(!ready[i].load()) {
            bool is_waiting = false;
            for (int j = i + 1; j < n; j++) {
                if (a[i][j] && !ready[j].load()) {
                    is_waiting = true;
                } else if (a[i][j] && ready[j].load()) {
                    x[i] -= a[i][j] * x[j] / a[i][i];
                    a[i][j] = 0;
                }
            }
            if(!is_waiting) {
                ready[i].store(true);
            }
        }
    }
    main_barrier.enter();
}

#endif //GAUSS_PARALLEL_GAUSS_H
