#include <iostream>
//#include <condition_variable>
#include <thread>
#include <vector>
#include <random>

//#include "cyclic_barrier.h"


#include <iostream>
#include <atomic>
#include <thread>
#include <vector>

class cyclic_barrier {
	std::atomic<bool> is_barrier_open;
	//std::mutex mtx;
	int size;
	int counter;
	std::atomic<bool> allow;
	std::atomic<bool> come_to_barrier_cond;

public:
	cyclic_barrier(int size_ = 10): size(size_), counter(0), allow(true) {}
	~cyclic_barrier() {}
	void enter() {
		//std::unique_lock<std::mutex> lock(mtx);
		//while (!allow) {
			//come_to_barrier_cond.wait(lock);
		//}
		++counter;
		if (counter == size){
			allow = false;
		}

		while (counter != size && allow) {
			barrier_not_open_cond.wait(lock);
		}
		if (counter == size) {
			counter--;
			barrier_not_open_cond.notify_all();
		} else {
			counter--;
		}
		if (counter == 0) {
			allow = true;
			come_to_barrier_cond.notify_all();
		}
	}
};



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