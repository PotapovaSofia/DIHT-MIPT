#ifndef _SPSC_RING_BUFFER_ABSTRACT_H_
#define _SPSC_RING_BUFFER_ABSTRACT_H_

template <typename T>
class spsc_ring_buffer_abstract {
public:
    spsc_ring_buffer_abstract() {};
    ~spsc_ring_buffer_abstract() {}

    virtual bool enqueue(T) = 0;
    virtual bool dequeue(T&) = 0;
};

#endif //_SPSC_RING_BUFFER_ABSTRACT_H_
