/*
 * g++ -std=c++11 -pthread -I /home/sonya/Downloads/boost_1_57_0 main.cpp
 * -L /home/sonya/Downloads/boost_1_57_0/libs/ -lboost_thread -lboost_system -o a
 */

#include <iostream>
#include <forward_list>
#include <boost/thread.hpp>
//#include "striped_hash_set.h"

#define THREADS_CNT 2

#include <iostream>
#include <forward_list>
#include <boost/thread.hpp>
#include <algorithm>
#include <thread>
#include <atomic>

#define GROWTH_FACTOR 2
#define LOAD_FACTOR 0.5

template<typename T, typename Hash = std::hash<T>>
class striped_hash_set {

    std::vector<std::forward_list<T>> buckets;
    std::vector<boost::shared_mutex> stripes;
    size_t growth_factor;
    double load_factor;
    std::atomic<size_t> set_size;
    Hash hasher;

public:

    striped_hash_set(size_t concurrency_level, size_t growth_factor_ = GROWTH_FACTOR,
                     double load_factor_ = LOAD_FACTOR) : buckets(concurrency_level), stripes(concurrency_level),
                                                          growth_factor(growth_factor_), load_factor(load_factor_),
                                                          set_size(0) {}

    ~striped_hash_set() {}

    void add(const T& e) {
        size_t hash = hasher(e);
        size_t stripe_index = get_stripe_index(hash);
        stripes[stripe_index].lock();
        size_t bucket_index = get_bucket_index(hash);
        if (find_entry_for(buckets[bucket_index], e) == buckets[bucket_index].end()) {
            buckets[bucket_index].push_front(e);
            size_t size = set_size.load();
            size++;
            set_size.store(size);
        }
        size_t size = set_size.load();
        if (get_current_load() >= load_factor) {
            stripes[stripe_index].unlock();
            resize(buckets.size());
        } else {
            stripes[stripe_index].unlock();
        }

    }

    bool contains(const T& e) {
        size_t hash = hasher(e);
        boost::shared_lock<boost::shared_mutex> lock(stripes[get_stripe_index(hash)]);
        size_t bucket_index = get_bucket_index(hash);
        return find_entry_for(buckets[bucket_index], e) != buckets[bucket_index].end();
    }

    void remove(const T& e) {
        size_t hash = hasher(e);
        boost::unique_lock<boost::shared_mutex> lock(stripes[get_stripe_index(hash)]);
        size_t bucket_index = get_bucket_index(hash);
        if (find_entry_for(buckets[bucket_index], e) != buckets[bucket_index].end()) {
            buckets[bucket_index].remove(e);
            size_t size = set_size.load();
            size--;
            set_size.store(size);
        }

    }

    void print() {
        for (auto bucket : buckets) {
            for (T& x : bucket) {
                std::cout << x << " -> ";
            }
            std::cout << std::endl;
        }
    }

private:

    typename std::forward_list<T>::iterator find_entry_for ( std::forward_list<T> &chain, const T& e) {
        return std::find(chain.begin(), chain.end(), e);
    }

    double get_current_load() {
        size_t size = set_size.load();
        return (double)size / buckets.size();
    }

    void resize(size_t expected_num_buckets) {
        boost::unique_lock<boost::shared_mutex> lock(stripes[0]);
        if (expected_num_buckets != buckets.size()) {
            return;
        }
        std::vector<boost::unique_lock<boost::shared_mutex>> locks;
        for (int i = 1; i < stripes.size(); i++) {
            locks.emplace_back(stripes[i]);
        }
        std::vector<std::forward_list<T>> new_table(buckets.size() * growth_factor);
        for (auto bucket: buckets) {
            for (T& x: bucket) {
                new_table[hasher(x) % new_table.size()].push_front(x);
            }
        }
        buckets.swap(new_table);
    }

    size_t get_stripe_index(size_t hash) {
        return hash % stripes.size();
    }

    size_t get_bucket_index(size_t hash) {
        return hash % buckets.size();
    }
};

/*
int main(){
    striped_hash_set<int> hash_table(16);
    std::vector<std::thread> threads;

    for (int i = 0; i < THREADS_CNT; i++) {
        threads.emplace_back([&](){
            for (int j = 0; j < 40; j++) {
                //std::random_device generator;
                //std::uniform_int_distribution<int> distribution(1, 100);
                //int k = distribution(generator);
                hash_table.add(j);
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
 */
#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>
#include <future>

#include "barrier.h"
#include "steady_timer.h"

#include "coarse_grained_hash_set.h"

//////////////////////////////////////////////////////////////////////////

using uint_t = std::uint64_t;

struct uint_hash {
    size_t operator ()(const uint_t x) const {
        return x * (x + 3) % 15487399;
    }
};

//////////////////////////////////////////////////////////////////////////

using hash_set_t = striped_hash_set<uint_t, uint_hash>;

//////////////////////////////////////////////////////////////////////////

// [start, end]
struct range {
    range(uint_t s, uint_t e) : start(s), end(e) {}

    uint_t start;
    uint_t end;

    uint_t length() const {
        return end - start + 1;
    }
};

std::vector<range> split_range(const range r, const size_t num_splits) {
    uint_t sub_range_length = r.length() / num_splits;

    std::vector<range> splits;

    uint_t curr_start = r.start;
    for (size_t i = 0; i + 1 < num_splits; ++i) {
        splits.push_back(range(curr_start, curr_start + sub_range_length - 1));
        curr_start += sub_range_length;
    }
    splits.push_back(range(curr_start, r.end));
    return splits;
}

// first member of arithmetic progression with step <step> in range <r>
uint_t get_start_num(range r, uint_t step) {
    if (r.start % step == 0) {
        return r.start;
    } else {
        return r.start + (step - r.start % step);
    }
}

void add_and_search_test(uint_t n, size_t num_insert_threads, size_t num_search_threads, size_t insert_step, size_t search_step) {
    hash_set_t hash_set(64);
    steady_timer timer;

    std::cout << "add nums..." << std::endl;

    std::vector<range> insert_ranges = split_range(range(1, n), num_insert_threads);

    timer.reset();

    std::vector<boost::thread> inserters;
    barrier start_inserts(num_insert_threads);
    for (size_t i = 0; i < num_insert_threads; ++i) {
        std::cout << i << "-th thread insert range: [" << insert_ranges[i].start << ", " << insert_ranges[i].end << "]" << std::endl;

        auto insert_nums_to_hash_set = [&hash_set, &start_inserts, insert_step](range insert_range) {
            uint_t u = get_start_num(insert_range, insert_step);

            start_inserts.enter();

            while (u <= insert_range.end) {
                hash_set.add(u);
                u += insert_step;
            }
        };
        inserters.emplace_back(insert_nums_to_hash_set, insert_ranges[i]);
    }

    for (auto& t : inserters) {
        t.join();
    }

    std::cout << "add completed: " << timer.seconds_elapsed() << " seconds" << std::endl;

    std::cout << "search nums..." << std::endl;

    std::vector<range> search_ranges = split_range(range(1, n), num_search_threads);

    timer.reset();

    barrier start_searches(num_search_threads);
    std::vector<std::future<uint_t>> sums_of_found_nums;
    for (size_t i = 0; i < num_search_threads; ++i) {
        std::cout << i << "-th thread search range: [" << search_ranges[i].start << ", " << search_ranges[i].end << "]" << std::endl;

        auto search_nums = [&hash_set, &start_searches, search_step](range search_range) -> uint_t {
            uint_t sum_of_found = 0;
            uint_t u = get_start_num(search_range, search_step);

            start_searches.enter();

            while (u <= search_range.end) {
                if (hash_set.contains(u)) {
                    sum_of_found += u;
                }
                u += search_step;
            }
            return sum_of_found;
        };
        sums_of_found_nums.push_back(std::async(std::launch::async, search_nums, search_ranges[i]));
    }

    uint_t total_sum_of_found_nums = 0;

    for (auto& search_result : sums_of_found_nums) {
        total_sum_of_found_nums += search_result.get();
    }

    std::cout << "search completed: " << timer.seconds_elapsed() << " seconds" << std::endl;

    uint_t expected_sum = 0;
    for (uint_t u = search_step; u <= n; u += search_step) {
        if (u % insert_step == 0) {
            expected_sum += u;
        }
    }

    std::cout << "sum of nums found in hash set: " << total_sum_of_found_nums << std::endl;
    std::cout << "expected sum: " << expected_sum << std::endl;
}

int main() {
    add_and_search_test(
            10000000, // n
            6, // num of insert threads
            7, // num of search threads
            9, // insert step
            7  // search step
    );

    return 0;
}