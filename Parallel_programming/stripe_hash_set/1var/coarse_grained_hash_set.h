#pragma once

#include <mutex>
#include <unordered_set>

template <typename T, class H = std::hash<T>>
class coarse_grained_hash_set {
public:
    coarse_grained_hash_set(size_t /* concurrency_level */)
    {}

    void add(const T& e) {
        std::lock_guard<mutex_t> lock(mtx_);
        hash_set_.insert(e);
    }

    bool contains(const T& e) {
        std::lock_guard<mutex_t> lock(mtx_);
        return hash_set_.find(e) != hash_set_.end();
    }

    void remove(const T& e) {
        std::lock_guard<mutex_t> lock(mtx_);
        hash_set_.erase(e);
    }

private:
    std::unordered_set<T> hash_set_;
    using mutex_t = std::mutex;
    mutex_t mtx_;
};

