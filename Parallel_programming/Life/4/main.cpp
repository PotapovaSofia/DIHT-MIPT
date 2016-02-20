#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <time.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <omp.h>

using namespace std;

vector<vector<bool> > world;
vector<vector<bool> > old_world;
int world_width, world_height, thread_cnt;
volatile int status = 0;
volatile int quit_sig = 0; 
//vector<int> live_cells;
//vector<int> worlds_cmp;
vector<int> start_iteration_flags;
volatile int iterations_left = 0;
volatile int unfinished_threads_counter = 0;

void* worker_fn(int thread_id, int border_top, int border_bottom) {
	while (true) {
		//printf("worker task: %d\n", thread_id);
		while (!quit_sig && !start_iteration_flags[thread_id]) {
			#pragma omp taskyield
			//printf("taskyield: %d\n", thread_id);
		}
		if (quit_sig) break;
		start_iteration_flags[thread_id]--;
		/*
		for (int i = border_top; i < border_bottom; i++) {
			for (int j = 0; j < world_width; ++j) {
				old_world[i][j] = world[i][j];
			}
		}
		*/
		//printf("calculation: %d\n", thread_id);
        for (int i = border_top; i < border_bottom; i++) {
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
		/*
        for (int i = border_top; i < border_bottom; i++) {
			for (int j = 0; j < world_width; ++j) {
				if (world[i][j] != old_world[i][j]) {
					worlds_cmp[thread_id] = 1;
				}
				if (world[i][j]) {
					live_cells[thread_id]++;
				}
			}
		}
		*/
         #pragma omp critical
        {
        	//printf("minimaster in critical: %d\n", thread_id);
            unfinished_threads_counter--;
            if (unfinished_threads_counter == 0) {
                unfinished_threads_counter = thread_cnt;
                old_world.clear();
            	swap(old_world, world);
                if (iterations_left > 0)
                    iterations_left--;
                if (iterations_left <= 0) {
                    iterations_left = 0;
                    status = 1;
                } else {
                    for (int i = 0; i < thread_cnt; i++) {
                        start_iteration_flags[i]++;
                    }
                }
                 /*
	            int w_cmp = 0, l_cells = 0;
	        	for (int i = 0; i < thread_cnt; ++i) {
	        		if (worlds_cmp[i]) w_cmp++;
	        		l_cells += live_cells[i];
	        	}
	        	if (!l_cells || !w_cmp) {
	        		cout << "Stoppage condition" << endl;
	        		status = 1;
	        		quit_sig = true;
	        	}
	        	*/
            }
            if (status == 1) {
            	cout << "New World: " << endl;
            	for (int i = 0; i < world_height; i++) {
			        for (int j = 0; j < world_width; j++) {
			            if (old_world[i][j]) {
			                std::cout << '+';
			            } else {
			                std::cout << ' ';
			            }
			            cout << ' ';
			        }
			        std::cout << endl;
			    }
			    std::cout << endl;
            }
        }
	}
}

int main () {
	omp_set_nested(1);
	#pragma omp parallel num_threads(2)
	{
		#pragma omp single
		{
			while(true) {
				string command;
				cin >> command;
				if (command == "START") {
					if (status) {
						cout << "Programm has already been started." << endl;
					} else {
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
						/*
						live_cells.resize(thread_cnt);
						for (int i = 0; i < thread_cnt; ++i) {
							live_cells[i] = 0;
						}
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
					    start_iteration_flags.reserve(thread_cnt);
                    	unfinished_threads_counter = thread_cnt;
						#pragma omp task
						{
							//printf("task\n");
			                #pragma omp parallel num_threads(thread_cnt)
			                {
			                	//printf("parallel\n");
			                    #pragma omp single
			                    {	
			                    	//printf("single\n");
			                        int remainder = world_height % thread_cnt;
			                        int prev_top_border = 0;
			                        for (int id = 0; id < thread_cnt; id++) {
			                            int bot_border = prev_top_border + (world_height / thread_cnt);
			                            if (remainder > 0) {
			                                bot_border++;
			                                remainder--;
			                            }
			                            start_iteration_flags[id] = 0;
			                            #pragma omp task
			                            worker_fn(id, prev_top_border, bot_border);
			                            prev_top_border = bot_border;
			                        }
			                    }
			                }
			            }
			            status = 1;
					}
				} else if (command == "STATUS") {
					if (!status) {
						cout << "Please, start the program." << endl;
					} else if (status == 1) {
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
					}
				} else if (command == "RUN") {
					if (!status) {
						cout << "Please, start the program" << endl;
					} else if (status == 1) {
						int iter_num; 
						cin >> iter_num;
						iterations_left = iter_num;	
						status = 2;
						for (int i = 0; i < thread_cnt; i++) {
                            start_iteration_flags[i]++;
                        }
					}
				} else if (command == "STOP") { 
					if (!status) {
						cout << "Please, start the program" << endl;
					} else if (status == 1) {
						cout << "Please, run" << endl;
					} else {
						iterations_left = 0;
						status = 1; 				
					}
				} else if (command == "QUIT") {
					if (!status) {
						cout << "Please, start the program" << endl;
					} else {
						quit_sig = true;
						#pragma omp taskwait
						break;
					}
				} else {
					cout << "Incorrect name of command" << endl;
				}
			}
		}
	}
	return 0;
}
