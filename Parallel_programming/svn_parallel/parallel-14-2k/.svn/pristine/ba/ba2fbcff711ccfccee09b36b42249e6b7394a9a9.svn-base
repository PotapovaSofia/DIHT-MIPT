#include <iostream>
#include <condition_variable>
#include <thread>
#include <vector>

#include "cyclic_barrier.h"

using namespace std;

int main() {
	int size = 4;
	cyclic_barrier barrier(size);
	vector<thread> threads;

	for (int i = 0; i < size; ++i) {
		threads.push_back(thread([&]{
			srand(time(NULL));
			this_thread::sleep_for(chrono::seconds(rand() % 10));
			for (int i = 0; i < 2; ++i) {
				barrier.enter();
				cout << this_thread::get_id() << endl;
			}
		}));
	}

	for (int i = 0; i < size; ++i) {
		if (threads[i].joinable()) {
			threads[i].join();
		}
	}

    return 0;
}