#ifndef LOCK_FREE_STACK_LOCK_FREE_STACK_H
#define LOCK_FREE_STACK_LOCK_FREE_STACK_H

#include <atomic>
#include <memory>

template <typename T>
class lock_free_stack {

    class node {
    public:
        std::shared_ptr<T> data;
        std::atomic<node*> next;
        node(T const& data_): data(std::make_shared<T>(data_)) {}
    };

    std::atomic<node*> head;
    std::atomic<unsigned> threads_in_pop;
    std::atomic<node*> to_be_deleted;

public:

    lock_free_stack() : head(nullptr), to_be_deleted(nullptr), threads_in_pop (0) {}

    ~lock_free_stack() {
        delete_nodes(head);
        delete_nodes(to_be_deleted);
    }

    void push(T const& data) {
        node* new_node = new node(data);
        node* next = new_node->next = head.load();
        while (!head.compare_exchange_weak(next, new_node)) {
            new_node->next.store(next);
        }
    }

    std::shared_ptr<T> pop() {
        ++threads_in_pop;
        node* old_head = head.load();
        while(old_head && !head.compare_exchange_weak(old_head, old_head->next.load()));
        std::shared_ptr<T> result;
        if (old_head) {
            result.swap(old_head->data);
        }

        if(old_head) {
            if (threads_in_pop == 1) {
               node* deleting_nodes = to_be_deleted.exchange(nullptr);
                if(!--threads_in_pop) {
                    delete_nodes(deleting_nodes);
                } else if(deleting_nodes) {
                    node* last = deleting_nodes;
                    while(node* next = last->next.load()) {
                        last = next;
                    }
                    last->next.store(to_be_deleted.load());
                    node* next = last->next.load();
                    while (!to_be_deleted.compare_exchange_weak(next, deleting_nodes)) {
                        last->next.store(next);
                    }
                }
                delete old_head;
            } else {
                old_head->next.store(to_be_deleted.load());
                node* next = old_head->next.load();
                while (!to_be_deleted.compare_exchange_weak(next, old_head)) {
                    old_head->next.store(next);
                }
                --threads_in_pop;
            }    
        }
        return result;
    }

private:

    void delete_nodes(node* nodes) {
        while(nodes) {
            node* next = nodes->next.load();
            delete nodes;
            nodes = next;
        }                    
    }
};

#endif //LOCK_FREE_STACK_LOCK_FREE_STACK_H
