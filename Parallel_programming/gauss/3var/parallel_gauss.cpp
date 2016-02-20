#include <vector>
#include <thread>
#include <algorithm>

#include "parallel_gauss.h"
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
    parallel_gauss(int, std::vector<std::vector<double>>, std::vector<double>&);
    ~parallel_gauss();
    bool is_solved();

private:
    void worker_fn(int);
    int get_start_row(int, int, bool);
    int get_start_row_for_back_sub(int, int);
};

parallel_gauss::parallel_gauss(int n_, std::vector<std::vector<double>> a_, std::vector<double>& x_):
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

parallel_gauss::~parallel_gauss() {
    for(auto& th: threads) {
        th.join();
    }
}

bool parallel_gauss::is_solved() {
    return flag;
}

void parallel_gauss::worker_fn(int thread_id) {
    for (int p = 0; p < n-1; p++) {
        for (int i = get_start_row(thread_id, p, true); i < n; i += thread_cnt) {
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
        for (int j = get_start_row(thread_id, p, false); j < n; j +=thread_cnt) {
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
        for (int i = get_start_row_for_back_sub(thread_id, p); i >= 0; i -= thread_cnt) {
            a[i][n] -= x[p] * a[i][p];
        }
        barrier.enter();
    }
    main_barrier.enter();
}

int parallel_gauss::get_start_row(int thread_id, int p, bool is_first) {
    if (is_first) {
        return p + ((thread_id >= p) ? ((thread_id - p) % thread_cnt) : (thread_cnt - (p - thread_id) % thread_cnt));
    } 
    return p + ((thread_id > p) ? ((thread_id - p) % thread_cnt) : (thread_cnt - (p - thread_id) % thread_cnt));
} 

int parallel_gauss::get_start_row_for_back_sub(int thread_id, int p) {
    return p - ((thread_id < p) ? (p - thread_id) % thread_cnt : (thread_cnt + (thread_id - p) % thread_cnt));
}

bool solve_system_of_linear_equations(std::vector<std::vector<double>> a, std::vector<double>& result) {
    parallel_gauss solution(a.size(), a, result);
    return solution.is_solved();
}