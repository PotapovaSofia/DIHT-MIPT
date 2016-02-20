

#include <iostream>
#include <vector>
#include <thread>

int main() {
    int n, x;
    std::vector<int> a;
    std::cin >> n;
    for (int i = 0; i < n; i++) {
        std::cin >> x;
        a.push_back(x);
    }
    int thread_cnt = 2;
    std::vector<std::thread> threads;
    for (int i = 0; i < thread_cnt; i++) {
        threads.emplace_back([&a, i, thread_cnt] () {
            for (int j = i; j < a.size(); j+=thread_cnt) {
                a[j] *= 2;
            }
        });
    }

    for (auto& thread : threads) {
            thread.join();
    }

    for (int i = 0; i < a.size(); i++) {
        std::cout << a[i] << " ";
    }
    std::cout << std::endl;
    
    
    return 0;
}