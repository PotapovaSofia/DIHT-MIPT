#include <iostream>
#include <condition_variable>
#include <thread>
#include <vector>
#include <random>

#include "cyclic_barrier.h"

void simple_test() {
	int size = 4;
	cyclic_barrier barrier(size);
	std::vector<std::thread> threads;

	for (int i = 0; i < size; ++i) {
		threads.emplace_back([&]{
			std::default_random_engine generator;
			std::uniform_int_distribution<int> distribution(1,10);
			std::this_thread::sleep_for(std::chrono::seconds(distribution(generator)));
			for (int i = 0; i < 10; ++i) {
				barrier.enter();
				std::cout << std::this_thread::get_id() << std::endl;
			}
		});
	}

	for (auto& th: threads) {
		th.join();
	}
}

void stress_test() {
	cyclic_barrier barrier(10);
	std::vector<std::thread> threads;

	for (int i = 0; i < 10; ++i) {
		threads.emplace_back([&]{
			for (int i = 0; i < 100000; ++i) {
				barrier.enter();
			}
		});
	}

	for (auto& th: threads) {
		th.join();
	}
}

int main() {
	simple_test();
	stress_test();

	return 0;
}