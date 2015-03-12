#include <iostream>
#include <functional>
#include <mutex>
#include <thread>
#include <vector>
#include <future>

#include "thread_pool.h"

#define TEST_NUMBERS_CNT 10

using namespace std;

int pow2(int x) {
    return x*x;
}

int main() {

    thread_pool<int> first_pool;
    vector<future<int> > first_answer;
    for (int i = 0; i < TEST_NUMBERS_CNT; i++) {
        first_answer.emplace_back(first_pool.execute(bind(pow2, i)));
    }
    for (int i = 0; i < first_answer.size(); i++) { //по непонятным для меня причинам здесь не работает foreach
        cout << first_answer[i].get() << endl;
    }

    return 0;
}