#include <iostream>
#include <forward_list>
#include <boost/thread.hpp>
#include <algorithm>
#include <thread>

#define GROWTH_FACTOR 2
#define LOAD_FACTOR 2

template<typename T, typename Hash = std::hash<T>>
class striped_hash_set {

    std::vector<std::forward_list<T>> buckets;
    std::vector<boost::shared_mutex> stripes;
    size_t growth_factor;
    double load_factor;
    size_t set_size;
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
        boost::unique_lock<boost::shared_mutex> lock(stripes[stripe_index]);
        size_t bucket_index = get_bucket_index(hash);
        if (find_entry_for(buckets[bucket_index], e) == buckets[bucket_index].end()) {
            buckets[bucket_index].push_front(e);
            set_size++;
        }
        if (get_current_load() >= load_factor) {
            stripes[stripe_index].unlock();
            resize(buckets.size());
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
            set_size--;
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
        return (double)set_size / buckets.size();
    }

    void resize(size_t expected_num_buckets) {
        boost::unique_lock<boost::shared_mutex> lock(stripes[0]);
        if (expected_num_buckets == buckets.size()) {
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
        std::cout << buckets.size();
    }

    size_t get_stripe_index(size_t hash) {
        return hash % stripes.size();
    }

    size_t get_bucket_index(size_t hash) {
        return hash % buckets.size();
    }
};
