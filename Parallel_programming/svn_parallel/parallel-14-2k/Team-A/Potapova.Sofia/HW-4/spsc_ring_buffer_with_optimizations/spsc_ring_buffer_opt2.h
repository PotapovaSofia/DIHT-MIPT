#ifndef _SPSC_RING_BUFFER_OPT2_H_
#define _SPSC_RING_BUFFER_OPT2_H_

#ifndef _SPSC_RING_BUFFER_NODE_T_H_
#define _SPSC_RING_BUFFER_NODE_T_H_

#define CACHE_LINE_SIZE 64

template <typename T>
struct node_t{
    T elem;
    char pad[CACHE_LINE_SIZE];
};

#endif //_SPSC_RING_BUFFER_NODE_T_H_

template <typename T>
class spsc_ring_buffer_opt2: public spsc_ring_buffer_abstract<T> {
    std::vector<node_t<T>> ring_buffer;
    std::atomic<size_t> head;
    char pad[CACHE_LINE_SIZE];
    std::atomic<size_t> tail;
    size_t capacity;

public:
    spsc_ring_buffer_opt2(size_t capacity_) :
            capacity(capacity_), head(0), tail(0), ring_buffer(capacity_) {}

    ~spsc_ring_buffer_opt2() {}

    bool enqueue(T elem) {
        size_t current_head = head.load(std::memory_order_relaxed);
        size_t current_tail = tail.load(std::memory_order_acquire);
        if (0 == (current_tail - current_head + 1) % capacity) {
            return false;
        }
        ring_buffer[current_tail].elem = elem;
        tail.store((current_tail + 1) % capacity);
        return true;
    }

    bool dequeue(T& elem) {
        size_t current_tail = tail.load(std::memory_order_relaxed);
        size_t current_head = head.load(std::memory_order_acquire);
        if (current_head == current_tail) {
            return false;
        }
        elem = ring_buffer[current_head].elem;
        head.store((current_head + 1) % capacity);
        return true;
    }
};

#endif //_SPSC_RING_BUFFER_OPT2_H_
