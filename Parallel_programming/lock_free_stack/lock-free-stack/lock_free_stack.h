#ifndef LOCK_FREE_STACK_LOCK_FREE_STACK_H
#define LOCK_FREE_STACK_LOCK_FREE_STACK_H

#include <atomic>
#include <memory>

template <typename T>
class lock_free_stack {

    class node {
    public:
        std::shared_ptr<T> data;
        node* next;
        node(T const& data_): data(std::make_shared<T>(data_)) {}
    };

    std::atomic<node*> head;
    std::atomic<unsigned> threads_in_pop;
    std::atomic<node*> to_be_deleted;

public:

    void push(T const& data) {
        node* new_node = new node(data);
        new_node->next = head.load();
        while(!head.compare_exchange_weak(new_node->next, new_node));
    }

    std::shared_ptr<T> pop() {
        ++threads_in_pop;
        node* old_head = head.load();
        while(old_head && !head.compare_exchange_weak(old_head, old_head->next));
        std::shared_ptr<T> result;
        if (old_head) {
            result.swap(old_head->data);
        }

        if (threads_in_pop == 1) {
            node* deleting_nodes = to_be_deleted.exchange(nullptr);
            if(!--threads_in_pop) {
                while(deleting_nodes) {
                    node* next_node = deleting_nodes->next;
                    delete deleting_nodes;
                    deleting_nodes = next_node;
                }
            } else if(deleting_nodes) {
                node* last = deleting_nodes;
                while(node* next = last->next) {
                    last = next;
                }
                last->next = to_be_deleted;
                while(!to_be_deleted.compare_exchange_weak(last->next, deleting_nodes));
            }
            delete old_head;
        } else {
            old_head->next = to_be_deleted;
            while(!to_be_deleted.compare_exchange_weak(old_head->next, old_head));
            --threads_in_pop;
        }

        return result;
    }
};

#endif //LOCK_FREE_STACK_LOCK_FREE_STACK_H
