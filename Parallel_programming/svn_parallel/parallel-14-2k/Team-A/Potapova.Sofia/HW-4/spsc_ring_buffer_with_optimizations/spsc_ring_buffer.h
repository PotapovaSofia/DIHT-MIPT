#ifndef _SPSC_RING_BUFFER_H_
#define _SPSC_RING_BUFFER_H_

template <typename T>
class spsc_ring_buffer: public spsc_ring_buffer_abstract<T> {
    std::vector<T> ring_buffer;
    std::atomic<size_t> head;
    std::atomic<size_t> tail;
    size_t capacity;

public:
    spsc_ring_buffer(size_t capacity_) :
            capacity(capacity_), head(0), tail(0), ring_buffer(capacity_) {}

    ~spsc_ring_buffer() {}

    bool enqueue(T elem) {
        size_t current_head = head.load(std::memory_order_relaxed);
        size_t current_tail = tail.load(std::memory_order_acquire);
        if (0 == (current_tail - current_head + 1) % capacity) {
            return false;
        }
        ring_buffer[current_tail] = elem;
        tail.store((current_tail + 1) % capacity,std::memory_order_relaxed);
        return true;
    }

    bool dequeue(T& elem) {
        size_t current_tail = tail.load(std::memory_order_relaxed);
        size_t current_head = head.load(std::memory_order_relaxed);
        if (current_head == current_tail) {
            return false;
        }
        elem = ring_buffer[current_head];
        head.store((current_head + 1) % capacity, std::memory_order_release);
        return true;
    }
};


#endif //_SPSC_RING_BUFFER_SPSC_RING_BUFFER_H_
