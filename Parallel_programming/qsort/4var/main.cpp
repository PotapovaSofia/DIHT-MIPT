#include <iostream>
#include <vector>
#include <algorithm>
#include "thread_pool.h"
#include "parallel_quick_sort.h"

#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>


///////////////////////////////////////////////////////////////////////////

class steady_timer {
public:
    steady_timer() {
        reset();
    }

    void reset() {
        start_ = std::chrono::steady_clock::now();
    }

    double seconds_elapsed() const {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast< std::chrono::duration<double> >(now - start_);
        return elapsed.count();
    }

private:
    std::chrono::steady_clock::time_point start_;
};

///////////////////////////////////////////////////////////////////////////

// include header with sorting routine

void parallel_sort(std::vector<int>& data) {
    parallel_quick_sort(data);
}

const size_t N = 10000000;

int main() {
    std::random_device rand_dev;
    std::mt19937 mt_engine(rand_dev());
    std::uniform_int_distribution<int> gen_uniform_ints(1, 1000000);

    std::vector<int> random_ints;
    random_ints.reserve(N);
    for (size_t i = 0; i < N; ++i) {
        random_ints.push_back(gen_uniform_ints(mt_engine));
    }

    std::vector<int> seq_sort_random_ints(random_ints);

    // parallel sort
    {
        steady_timer timer;
        parallel_sort(random_ints);
        double sort_time = timer.seconds_elapsed();
        std::cout << "parallel sort completed in " << sort_time << " seconds: " << std::is_sorted(random_ints.begin(), random_ints.end()) << std::endl;
    }

    // sequential sort
    {
        steady_timer timer;
        std::sort(seq_sort_random_ints.begin(), seq_sort_random_ints.end());
        double sort_time = timer.seconds_elapsed();
        std::cout << "sequential sort completed in " << sort_time << " seconds: " << std::is_sorted(seq_sort_random_ints.begin(), seq_sort_random_ints.end()) << std::endl;
    }

    for (size_t i = 0; i < N; ++i) {
        if (random_ints[i] != seq_sort_random_ints[i]) {
            std::cout << "mismatch found at index " << i << ": " << random_ints[i] << " != " << seq_sort_random_ints[i] << std::endl;
        }
    }

    return 0;
}