#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <time.h>
#include <pthread.h>
#include "cyclic_barrier.h"
#include <sys/time.h>
#include <unistd.h>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;


class atomic_t {
	int data;
	pthread_rwlock_t rwl;
public: 
	atomic_t() {
		data = 0;
		pthread_rwlock_init(&rwl, NULL);
	}
	atomic_t (int _data): data(_data) {
		pthread_rwlock_init(&rwl, NULL);
	}
	~atomic_t () {
		pthread_rwlock_destroy(&rwl);
	}
	void set(int _data) {
		pthread_rwlock_wrlock(&rwl);
		data = _data;
		pthread_rwlock_unlock(&rwl);
	}
	int get() {
		int ret;
		pthread_rwlock_rdlock(&rwl);
		ret = data;		
		pthread_rwlock_unlock(&rwl);
		return ret;
	}
	void fetch_add(int _data) {
		pthread_rwlock_wrlock(&rwl);
		data += _data;
		pthread_rwlock_unlock(&rwl);
	}
};

vector<vector<bool> > world;
vector<vector<bool> > old_world;
vector<pthread_t> threads;
pthread_t master;
int world_width, world_height, thread_cnt;
atomic_t iter_num, worlds_cmp, stop_sig, status_sig, run_sig, quit_sig, iter_current;
vector<int> live_cells;
cyclic_barrier barrier;
clock_t time_a, time_b;
void abort_prg(const char* error) {
    cerr << "ERROR: " << error;
	for (int i = 0; i < thread_cnt; ++i) {
		pthread_join(threads[i], NULL);
	}
	pthread_join(master, NULL);
    exit(1);
}

int get_border(int k, int cnt, int n) {
    k = (k % cnt + cnt) % cnt;
    if (k < n % cnt)
       return k * (n / cnt + 1);
    int size = n / cnt;
    return (n % cnt) * (size + 1) + (k - n % cnt) * size;
}
/*
void get_border(int id, int h, int start) {
	if (id < world_height % thread_cnt)
		h = 
		start = id
}
*/
void * master_fn (void * arg) {
	while (!quit_sig.get()) if (run_sig.get() && !stop_sig.get()) {
		int N = iter_num.get();
		for (int i = 0; i < N; ++i) {
			cout << "iter " << i << endl; 
			iter_current.fetch_add(1);
			worlds_cmp.set(0);
			for (int i = 0; i < 3; ++i) {
				barrier.enter();
			}
			int live_cells_cnt = 0;
		    for (int i = 0; i < thread_cnt; ++i) {
		    	live_cells_cnt +=live_cells[i];
		    }
			if (!live_cells_cnt) {
		        cout << "All cells died.  Please, quit the program." << endl;
		        quit_sig.set(1);
		    } else if (!worlds_cmp.get()) {
		        cout << "Stoppage configuration. Please, quit the program." << endl;
		        quit_sig.set(1);
		    }
		    if (stop_sig.get()) {
		    	cout << "Current iteration: " << iter_current.get() << endl;
		    }
		    barrier.enter();
		    if (status_sig.get()) {
		    /*
		     * Print world.
		     */
			    for (int i = 0; i < world_height; i++) {
			        for (int j = 0; j < world_width; j++) {
			            if (world[i][j]) {
			                std::cout << '+';
			            } else {
			                std::cout << ' ';
			            }
			            cout << ' ';
			        }
			        std::cout << endl;
			    }
			    std::cout << endl;
				status_sig.set(0);
			}
			if (quit_sig.get() || stop_sig.get()) {
				break;
			}
			
			for (int i = 0; i < world_height; i++) {
		        for (int j = 0; j < world_width; j++) {
		            if (world[i][j]) {
		                std::cout << '+';
		            } else {
		                std::cout << ' ';
		            }
		            cout << ' ';
		        }
		        std::cout << endl;
		    }
		    std::cout << endl;
		    
		    usleep(1000000);
		    barrier.enter();
		}
		run_sig.set(0);
		barrier.enter();
	}
}

void* worker_fn(void* arg) {
	while (!quit_sig.get()) if (run_sig.get()) {
		int thread_id = (intptr_t) arg;
		int N = iter_num.get();
		int thread_height = get_border(thread_id);
		for (int k = 0; k < N; ++k) {
			/*
			 * Copy world.
			 */
			for (int i = thread_id; i < world_height; i+= thread_cnt) {
				for (int j = 0; j < world_width; ++j) {
					old_world[i][j] = world[i][j];
				}
			}
	        barrier.enter();
	        /*
	         * Calculate new generation.
	         */
	        for (int i = thread_id; i < world_height; i+= thread_cnt) {
				for (int j = 0; j < world_width; ++j) {
					int nb[8][2];
				    int x, y, p = 0, alive_nb = 0;
				    for (int l = i - 1; l <= i + 1; l++) {
				        for (int m = j - 1; m <= j + 1; m++) {
				            if (l == i && m == j) continue;
				            nb[p][0] = l;
				            nb[p][1] = m;
				            p++;
				        }
				    }
				    for (int p = 0; p < 8; p++) {
				        x = nb[p][0];
				        y = nb[p][1];

				        if (x < 0) x += world_height; 
				        if (y < 0) y += world_width;
				        if (x >= world_height) x %= world_height;
				        if (y >= world_width) y %= world_width;
				        if (old_world[x][y]) alive_nb++;
				    }
		            if (!old_world[i][j]) {
		                if (alive_nb == 3) {
		                    world[i][j] = 1;
		                }
		            } else {
		                if (alive_nb < 2 || alive_nb > 3) {
		                    world[i][j] = 0;
		                }
		            }
				}
			}
	        barrier.enter();
	        /*
	         * Check on stoppage condition.
	         */
	        for (int i = thread_id; i < world_height; i+= thread_cnt) {
				for (int j = 0; j < world_width; ++j) {
					if (world[i][j] != old_world[i][j]) {
						worlds_cmp.set(1);
					}
					if (world[i][j]) {
						live_cells[thread_id]++;
					}
				}
			}
	        barrier.enter();
	        barrier.enter();
	        if (stop_sig.get() || quit_sig.get()) {
		    	break;
		    }
	        /*
	         * Print.
	         */
	        barrier.enter();
		}
		barrier.enter();
	}

}

int main () {
	bool start = false, run = false;
	while(true) {
		string command;
		cin >> command;
		if (command == "START") {
			if (start) {
				cout << "Programm has already been started." << endl;
			} else {
				start = true;
				int mode;
				cout << "Select mode: 1 - CSV; 2 - Random" << endl;
				cin >> mode;
				if (mode == 1) {
					string input, filename;
					cout << "Enter filename" << endl;
					cin >> filename;
					ifstream file(filename.c_str());
					if (!file.is_open()) {
						cout << "ERROR: open file" << endl;
						exit(0);
					}
					int i = 0;
					while (getline(file, input)) {
						vector<bool> line;
		                stringstream line_stream(input);
		                string cell;
		                while (getline(line_stream, cell, ',')) {
		                    line.push_back(cell == "1" ? 1 : 0);
		                }
						if (i == 0) {
							world_width = line.size();
						} else {
							if (line.size() != world_width) {
								cout << "Invalid csv format: incorrect width." << endl;
							}
						}
						world.push_back(line);
						i++;

					}
					world_height = world.size();
				} else {
					cin >> world_width >> world_height;
				}
				old_world.resize(world_height);
				for (int i = 0; i < world_height; ++i) {
					old_world[i].resize(world_width);
				}
				if (mode == 2) {
					for (int i = 0; i < world_height; i++) {
						for (int j = 0; j < world_width; ++j) {
							//srand (time(NULL));
							if ((rand()%10000) % 2) {
								world[i][j] = 1;
							}
						}
					}
				}
				cout << "Enter thread count" << endl;
				cin  >> thread_cnt;	
				threads.resize(thread_cnt);
				live_cells.resize(thread_cnt);
				for (int i = 0; i < thread_cnt; ++i) {
					live_cells[i] = 0;
				}
				
				for (int i = 0; i < world_height; i++) {
			        for (int j = 0; j < world_width; j++) {
			            if (world[i][j]) {
			                std::cout << '+';
			            } else {
			                std::cout << ' ';
			            }
			            cout << ' ';
			        }
			        std::cout << endl;
			    }
			    std::cout << endl;

			    barrier.set_size(thread_cnt + 1);
				if (pthread_create(&master, NULL, master_fn, NULL)) {
					abort_prg("pthread_create");
				}
				for (int id = 0; id < thread_cnt; ++id) {
					if (pthread_create(&threads[id], NULL, worker_fn, (void *)id)) {
						abort_prg("pthread_create");
					}
				}
			}
		} else if (command == "STATUS") {
			if (!start) {
				cout << "Please, start the program." << endl;
			} else {
				if (stop_sig.get() || quit_sig.get()) {
					for (int i = 0; i < world_height; i++) {
				        for (int j = 0; j < world_width; j++) {
				            if (world[i][j]) {
				                std::cout << '+';
				            } else {
				                std::cout << ' ';
				            }
				            cout << ' ';
				        }
				        std::cout << endl;
				    }
				    std::cout << endl;
					status_sig.set(0);
				} else {
					status_sig.set(1);
				}
			}
		} else if (command == "RUN") {
			if (!start) {
				cout << "Please, start the program" << endl;
			} else {
				time_a = clock();
				run = true;
				int _iter_num; 
				cin >> _iter_num;
				iter_num.set(_iter_num);
				run_sig.set(1);
				stop_sig.set(0);
			}
		} else if (command == "STOP") { 
			if (!start) {
				cout << "Please, start the program" << endl;
			} else {
				if (!quit_sig.get()) {
					run_sig.set(0);
					stop_sig.set(1);
				} else {
					cout << "Current iteration: " << iter_current.get() << endl;
				}
				
			}
		} else if (command == "QUIT") {
			if (!start) {
				cout << "Please, start the program" << endl;
			} else {
				if (!quit_sig.get()) {
					quit_sig.set(1);
				}
				if (pthread_join(master, NULL)) {
					abort_prg("pthread_join");
				}
				for (int i = 0; i < thread_cnt; ++i) {
					if (pthread_join(threads[i], NULL)) {
						abort_prg("pthread_join");
					}
				}
				break;
				}
		} else {
			cout << "Incorrect name of command" << endl;
		}
	}
	return 0;
}
