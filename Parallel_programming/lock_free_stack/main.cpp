#include <iostream>
#include <vector>
#include <thread>
#include <random>
//#include "lock_free_stack.h"

#define THREADS_CNT 4

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

    void push(T const& data) {
        node* new_node = new node(data);
        new_node->next.store(head.load());
        node* next = new_node->next.load();
        while(!head.compare_exchange_weak(next, new_node));
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
                    node* next_node = deleting_nodes->next.load();
                    delete deleting_nodes;
                    deleting_nodes = next_node;
                }
            } else if(deleting_nodes) {
                node* last = deleting_nodes;
                while(node* next = last->next.load()) {
                    last = next;
                }
                last->next.store(to_be_deleted);
                node* next = last->next.load();
                while(!to_be_deleted.compare_exchange_weak(next, deleting_nodes));
                last->next.store(next);
            }
            delete old_head;
        } else {
            old_head->next.store(to_be_deleted);
            node* next = old_head->next.load();
            while(!to_be_deleted.compare_exchange_weak(next, old_head));
            old_head->next.store(next);
            --threads_in_pop;
        }
        return result;
    }
};


int main() {
    lock_free_stack<int> stack;
    std::vector<std::thread> threads;
    for(int i = 0; i < THREADS_CNT; i++) {
        threads.emplace_back([&]{
            for (int j = 0; j < 10000; j++) {
                std::random_device generator;
                std::uniform_int_distribution<int> distribution(1, 100);
                int k = distribution(generator);
                stack.push(k);
                //std::cout << "push   " << k << std::endl;
                std::shared_ptr<int> c = stack.pop();
                //std::cout << "pop\n";
                if(c) {
                    //std:: cout << std::this_thread::get_id() << ": " << *c << std::endl;
                } else {
                    //std:: cout << std::this_thread::get_id() << ": -" << std::endl;
                }

            }
        });
    }
    for (auto& th: threads) {
        th.join();
    }
    return 0;
}