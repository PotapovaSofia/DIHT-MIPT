#include <mutex>
#include <thread>
#include <limits.h>

class hierarchical_mutex {
    std::mutex m_mutex;
    int const mutex_hierarchy_level;
    int prev_mutex_hierarchy_level;
    static thread_local int this_thread_hierarchy_level;

public:
    hierarchical_mutex(unsigned int level): mutex_hierarchy_level(level), prev_mutex_hierarchy_level(0) {}

    void lock() {
        check_level_for_correctness();
        m_mutex.lock();
        update_level();
    }

    void unlock() {
        if (this_thread_hierarchy_level != mutex_hierarchy_level) {
            throw std::logic_error("incorrect level value");
        }
        this_thread_hierarchy_level = prev_mutex_hierarchy_level;
        m_mutex.unlock();
    }

    bool try_lock() {
        check_level_for_correctness();
        if (!m_mutex.try_lock())
            return false;
        update_level();
        return true;
    }

private:
    void check_level_for_correctness() {
        if (mutex_hierarchy_level >= this_thread_hierarchy_level)
            throw std::logic_error("incorrect level value");
    }

    void update_level() {
        prev_mutex_hierarchy_level = this_thread_hierarchy_level;
        this_thread_hierarchy_level = mutex_hierarchy_level;
    }
};

thread_local int hierarchical_mutex::this_thread_hierarchy_level(INT_MAX);