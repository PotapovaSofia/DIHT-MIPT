#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <algorithm>
#include <set>
#include <mpi.h>


#define vpii vector<point>
#define pb push_back
#define x first 
#define y second
#define spii set<point> 

using namespace std;

struct point {
    int x, y;
	bool operator!= (const point &p) const {
			return x != p.x || (x == p.x && y != p.y);
	}    
	bool operator< (const point &p) const {
			return x < p.x || (x == p.x && y < p.y);
	}
};


point mp(int a, int b) {
	point p;
	p.x = a;
	p.y = b;
	return p;
}

int nb_x[] = {1,  1,  0, -1, -1, -1,  0,  1, 0};
int nb_y[] = {0,  1,  1,  1,  0, -1, -1, -1, 0};

int get_worker_id(int k, int cnt, int n) {
    k = (k % n + n) % n;
    int q = (n / cnt + 1) * (n % cnt);
    if (k < q)
        return k / (n / cnt + 1);
    return (k - q) / (n / cnt) + (n % cnt);
}

int get_border(int k, int cnt, int n) {
    k = (k % cnt + cnt) % cnt;
    if (k < n % cnt)
       return k * (n / cnt + 1);
    int size = n / cnt;
    return (n % cnt) * (size + 1) + (k - n % cnt) * size;
}

int main (int argc, char** argv) {
	if (argc != 4) {
		printf("Not enough arguments\n");
		exit(0);
	}
	MPI_Init(&argc, &argv);
	int proc_id;
	MPI_Comm_rank(MPI_COMM_WORLD, &proc_id);
	int proc_cnt;
	MPI_Comm_size(MPI_COMM_WORLD, &proc_cnt);

	int world_width = atoi(argv[1]);
	int world_height = atoi(argv[2]);
	int iter_cnt = atoi(argv[3]);

	/* create a type for struct points */
    const int nitems = 2;
    int          blocklengths[2] = {1,1};
    MPI_Datatype types[2] = {MPI_INT, MPI_INT};
    MPI_Datatype mpi_point_type;
    MPI_Aint     offsets[2];

    offsets[0] = offsetof(point, x);
    offsets[1] = offsetof(point, y);

    MPI_Type_create_struct(nitems, blocklengths, offsets, types, &mpi_point_type);
    MPI_Type_commit(&mpi_point_type);

	if (proc_id == proc_cnt - 1) {
		printf("Init World:\n");
		/*
		 * Init
		 */
		spii world;
		int alive_cells = min(max(10, world_width * world_height / 3), 100000);
		for (int i = 0; i < alive_cells; ++i) {
			world.insert(mp(rand() % world_width, 
				rand() % world_height));
		}

		for (int i = 0; i < world_height; ++i) {
	        for (int j = 0; j < world_width; ++j) {
	            if (world.find(mp(i, j)) != world.end()) {
	                printf("+ ");
	            } else {
	                printf("- ");
	            }
	        }
	        printf("\n");
    	}
		/*
		 * Send.
		 */
		vector<vpii > send_buf(proc_cnt);
		spii::iterator it = world.begin();
		for (; it != world.end(); ++it) {
			int id = get_worker_id(it->first, proc_cnt - 1, world_height);
            send_buf[id].push_back(*it);
            int under_id = get_worker_id(it->first - 1, proc_cnt - 1, world_height);
            int above_id = get_worker_id(it->first + 1, proc_cnt - 1, world_height);
            if (id != under_id) send_buf[under_id].push_back(*it);
            if (id != above_id) send_buf[above_id].push_back(*it);
		}
		for (int id = 0; id < proc_cnt - 1; ++id) {
			int size = send_buf[id].size();
    		MPI_Send(&size, 1, MPI_INT, id, 0, MPI_COMM_WORLD);
    		MPI_Bsend(&send_buf[id][0], size, mpi_point_type, id, 1, MPI_COMM_WORLD);
        }
		world.clear();
		
		for (int iter = 0; iter < iter_cnt; ++iter) {
			int alive_cells = 0, differ = 0;
			for (int id = 0; id < proc_cnt - 1; ++id) {
				int sum;
				MPI_Recv(&sum, 1, MPI_INT, id, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				alive_cells += sum;
				int cmp;
				MPI_Recv(&cmp, 1, MPI_INT, id, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				differ += cmp;
			}
			int stop_cnd = (!alive_cells || !differ) ? 1 : 0;
			for (int id = 0; id < proc_cnt - 1; ++id) {
				MPI_Send(&stop_cnd, 1, MPI_INT, id, 0, MPI_COMM_WORLD);
			}
			if (stop_cnd) {
				printf("Stoppage condition on iteration %d\n", iter);
				break;
			}
		}
		
		/*
		 * Receive.
		 */
		for (int id = 0; id < proc_cnt - 1; ++id) {
			int size;
			MPI_Recv(&size, 1, MPI_INT, id, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		    vpii recv_buf(size);
		    MPI_Recv(&recv_buf[0], size, mpi_point_type, id, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            for (int j = 0; j < (int)recv_buf.size(); ++j) {
                world.insert(recv_buf[j]);
            }
        }
        /*
         * Print.
         */
        printf("New World:\n");
        for (int i = 0; i < world_height; ++i) {
	        for (int j = 0; j < world_width; ++j) {
	            if (world.find(mp(i, j)) != world.end()) {
	                printf("+ ");
	            } else {
	                printf("- ");
	            }
	        }
	        printf("\n");
    	}
	} else {
		int size;
		MPI_Recv(&size, 1, MPI_INT, proc_cnt - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	    vpii recv_buf(size);
	    MPI_Recv(&recv_buf[0], size, mpi_point_type, proc_cnt - 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		spii world, old_world;
		int border_top = get_border(proc_id, proc_cnt - 1, world_height);
		int border_bottom = get_border(proc_id + 1, proc_cnt - 1, world_height);
		if (border_bottom < border_top) {
			border_bottom = world_height;
		}
		for (int i = 0; i < (int)recv_buf.size(); ++i) {
            old_world.insert(recv_buf[i]);
            //printf("proc: %d, (%d, %d)\n", proc_id, recv_buf[i].x, recv_buf[i].y);
        }
		
		for (int iter = 0; iter < iter_cnt; ++iter) {
			/*
			 * Calculate.
			 */
			spii::iterator it = old_world.begin();
			for (; it != old_world.end(); ++it) {
				for (int i = 0; i < 9; ++i) {
					int x = (it->x + nb_x[i] + world_height) % world_height;
					int y = (it->y + nb_y[i] + world_width) % world_width;
					if (border_top <= x && border_bottom > x) {
						int alive_nb = 0;
						for (int j = 0; j < 8; ++j) {
							int x_ = (x + nb_x[j] + world_height) % world_height;
							int y_ = (y + nb_y[j] + world_width) % world_width;
							if (old_world.find(mp(x_, y_)) != old_world.end()) {
								alive_nb++;
							}
						}
						if (old_world.find(mp(x, y)) != old_world.end()) {
		                	if (alive_nb == 3 || alive_nb == 2) {
		                    	world.insert(mp(x, y));
		                	}
		            	} else {
		                	if (alive_nb == 3) {
		                    	world.insert(mp(x, y));
		                    }
		                }
					}
				}
			}
			/*
	         * Check on stoppage condition.
	         */
	         
	        int cmp = 0, alive_cells = world.size();
	        spii::iterator old_it = old_world.begin();
	        it = world.begin();
	        for (; old_it != old_world.end() && it != world.end(); ++it, ++old_it) {
	        	if (*it != *old_it) {
	        		cmp++;
	        	}
	        }
	        MPI_Send(&alive_cells, 1, MPI_INT, proc_cnt - 1, 0, MPI_COMM_WORLD);
	        MPI_Send(&cmp, 1, MPI_INT, proc_cnt - 1, 0, MPI_COMM_WORLD);
	        int stop_cnd;
			MPI_Recv(&stop_cnd, 1, MPI_INT, proc_cnt - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			if (stop_cnd) break;
	       
			/*
			 * Send.
			 */
			vpii under_row, above_row;
			it = world.begin();
			for (; it != world.end() && it->x == border_top; ++it) {
				above_row.pb(*it);
			}
			it = world.end();
			if (world.size()) {
				it--;
				for (; it != world.begin() && it->x == border_bottom - 1; --it) {
					under_row.pb(*it);
				}
				if (it->x == border_bottom - 1) {
					under_row.pb(*it);	
				}
			}
			size = above_row.size();
    		MPI_Send(&size, 1, MPI_INT, (proc_id - 1 + proc_cnt - 1) % (proc_cnt - 1), 0, MPI_COMM_WORLD);
    		MPI_Bsend(&above_row[0], size, mpi_point_type, (proc_id - 1 + proc_cnt - 1) % (proc_cnt - 1), 1, MPI_COMM_WORLD);
    		size = under_row.size();
    		MPI_Send(&size, 1, MPI_INT, (proc_id + 1) % (proc_cnt - 1), 0, MPI_COMM_WORLD);
    		MPI_Bsend(&under_row[0], size, mpi_point_type, (proc_id + 1) % (proc_cnt - 1), 1, MPI_COMM_WORLD);
			/*
			 * Receive.
			 */
			MPI_Recv(&size, 1, MPI_INT, (proc_id - 1 + proc_cnt - 1) % (proc_cnt - 1), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		    above_row.clear();
		    above_row.resize(size);
		    MPI_Recv(&above_row[0], size, mpi_point_type, (proc_id - 1 + proc_cnt - 1) % (proc_cnt - 1), 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			MPI_Recv(&size, 1, MPI_INT, (proc_id + 1) % (proc_cnt - 1), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		    under_row.clear();
		    under_row.resize(size);
		    MPI_Recv(&under_row[0], size, mpi_point_type, (proc_id + 1) % (proc_cnt - 1), 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			
			for (int i = 0; i < (int)above_row.size(); ++i) {
				world.insert(above_row[i]);
			}
			for (int i = 0; i < (int)under_row.size(); ++i) {
				world.insert(under_row[i]);
			}
			
			old_world.clear();
            swap(old_world, world);
		}
		vpii send_buf;
		spii::iterator iit = old_world.begin();
		for (; iit != old_world.end(); ++iit) {
			send_buf.pb(*iit);
		}
		size = send_buf.size();
		MPI_Send(&size, 1, MPI_INT, proc_cnt - 1, 0, MPI_COMM_WORLD);
		MPI_Bsend(&send_buf[0], size, mpi_point_type, proc_cnt - 1, 1, MPI_COMM_WORLD);
	}
	MPI_Type_free(&mpi_point_type);
	MPI_Finalize();
	return 0;
}
