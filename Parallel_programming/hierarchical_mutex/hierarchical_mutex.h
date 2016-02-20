#include <mutex>
#include <thread>
#include <limits.h>

class hierarchical_mutex {
    std::mutex m_mutex;
    int const set_level;
    int prev_level;
    static thread_local int current_level;

public:
    hierarchical_mutex(unsigned int level): set_level(level), prev_level(0) {}

    void lock() {
        check_level_for_correctness();
        m_mutex.lock();
        update_level();
    }

    void unlock() {
        if (current_level != set_level) {
            throw std::logic_error("incorrect level value");
        }
        current_level = prev_level;
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
        if (set_level >= current_level)
            throw std::logic_error("incorrect level value");
    }

    void update_level() {
        prev_level = current_level;
        current_level = set_level;
    }
};

thread_local int hierarchical_mutex::current_level(INT_MAX);