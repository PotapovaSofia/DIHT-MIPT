#include <iostream>
#include <functional>
#include <mutex>
#include <thread>
#include <vector>
#include <future>

#include "thread_pool.h"

#define TEST_NUMBERS_CNT 10

int pow2(int x) {
    return x*x;
}

int fn_with_exception() {
    throw "test exception";
}

int main() {

    thread_pool<int> first_pool;
    std::vector<std::future<int>> first_answer;
    for (int i = 0; i < TEST_NUMBERS_CNT; i++) {
        first_answer.emplace_back(first_pool.submit(std::bind(pow2, i)));
    }
    for (auto& i: first_answer) {
        try {
            std::cout << i.get() << std::endl;
        } catch (std::future_error const & e) {
            std::cerr << "Future error: " << e.what() << std::endl;
        } catch (std::exception const & e) {
            std::cerr << "Standard exception: " << e.what() << std::endl;
        } catch (...) {
            std::cerr << "Unknown exception." << std::endl;
        }
    }

    auto answer = first_pool.submit(fn_with_exception);
    try {
        std::cout << answer.get() << std::endl;
    } catch (const char* e) {
        std::cerr << e << std::endl;
    }

    return 0;
}