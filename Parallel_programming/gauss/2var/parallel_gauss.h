#ifndef GAUSS_PARALLEL_GAUSS_H
#define GAUSS_PARALLEL_GAUSS_H

#include <vector>
#include <thread>
#include <algorithm>

#include "cyclic_barrier.h"

#define EPS 1e-9

class parallel_gauss{
    int n;
    std::vector<std::vector<double>> a;
    std::vector<double>& x;
    int thread_cnt;
    std::vector<std::thread> threads;
    cyclic_barrier barrier;
    cyclic_barrier main_barrier;
    std::vector<std::pair<double, int>> max;
    bool flag;

public:
    parallel_gauss(int n_, std::vector<std::vector<double>> a_, std::vector<double>& x_):
            n(n_), a(a_), x(x_), flag(true) {
        thread_cnt = std::thread::hardware_concurrency();
        if (thread_cnt == 0) {
            thread_cnt = 4;
        }
        barrier.set_size(thread_cnt);
        main_barrier.set_size(thread_cnt+1);
        for (int i = 0; i < thread_cnt; i++) {
            max.emplace_back(0.0, 0);
        }
        for (int i = 0; i < thread_cnt; i++) {
            threads.emplace_back(&parallel_gauss::worker_fn, this, i);
        }
        main_barrier.enter();
    }

    ~parallel_gauss() {
        for(auto& th: threads) {
            th.join();
        }
    }

    bool is_solved() {
        return flag;
    }

private:
    void worker_fn(int thread_id) {
        for (int p = 0; p < n-1; p++) {
            int l = (thread_id - p) % thread_cnt;
            int m = (l >= 0) ? l : (thread_cnt + l);
            for (int i = p + m; i < n; i += thread_cnt) {
                if (abs(a[i][p]) > max[thread_id].first) {
                    max[thread_id].first = abs(a[i][p]);
                    max[thread_id].second = i;
                }
            }
            barrier.enter();
            if(thread_id == 0) {
                auto it_max = std::max_element(max.begin(), max.end());
                if (abs(it_max->first) < EPS) {
                    flag = false;
                    return;
                }
                std::swap(a[p], a[it_max->second]);
            }
            barrier.enter();
            m = (l > 0) ? l : (thread_cnt + l);
            for (int j = m + p; j < n; j +=thread_cnt) {
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
        for (int p = n - 1; p >= 0; p--) {
            if (thread_id == p %  thread_cnt) {
                x[p] = a[p][n] / a[p][p];
            }
            barrier.enter();
            int l = (thread_id - p) % thread_cnt;
            int m = (l < 0) ? (-l) : (thread_cnt + l);
            for (int i = p - m; i >= 0; i -= thread_cnt) {
                a[i][n] -= x[p] * a[i][p];
            }
            barrier.enter();
        }
        main_barrier.enter();
    }
};

#endif //GAUSS_PARALLEL_GAUSS_H