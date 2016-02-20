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

using namespace std;

struct point {
    int fst_cell_cnt, snd_cell_cnt;
    int nut_cntr, fst_cntr, snd_cntr; 
    //vector<int> strv_rnd;
};


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

	MPI_Init(&argc, &argv);//////////////////////////????????????????????????????????????
	int proc_id;
	MPI_Comm_rank(MPI_COMM_WORLD, &proc_id);
	int proc_cnt;
	MPI_Comm_size(MPI_COMM_WORLD, &proc_cnt);

	int world_width, world_height, iter_cnt, nut_recover_rate, fst_excr_rate, snd_excr_rate;
	int nut_eat_rate, fst_eat_rate, snd_eat_rate, nut_segment_cntr, fst_segment_cntr, snd_death_cntr;
	int root = 0;
	vector<vector<point>> world, old_world;
	

	const int nitems = 5;
    int          blocklengths[] = {1,1,1,1,1};
    MPI_Datatype types[5] = {MPI_INT, MPI_INT, MPI_INT, MPI_INT, MPI_INT};
    MPI_Datatype mpi_point_type;
    MPI_Aint     offsets[5];

    offsets[0] = offsetof(point, fst_cell_cnt);
    offsets[1] = offsetof(point, snd_cell_cnt);
    offsets[2] = offsetof(point, nut_cntr);
    offsets[3] = offsetof(point, fst_cntr);
    offsets[4] = offsetof(point, snd_cntr);

    MPI_Type_create_struct(nitems, blocklengths, offsets, types, &mpi_point_type);
    MPI_Type_commit(&mpi_point_type);

 	
    

	if (proc_id == root) {
		vector<vector<point>> init_world(world_height);
		for (int i = 0; i < world_height; ++i) {
			init_world.push_back(vector<point>(world_width));
		}


		freopen("input1.txt", "r", stdin);
		cin >> world_width >> world_height;
		//parse init_world
		freopen("input2.txt", "r", stdin);
		cin >> nut_recover_rate >> fst_excr_rate >> snd_excr_rate 
			>> nut_eat_rate >> fst_eat_rate >> snd_eat_rate 
			>> nut_segment_cntr >> fst_segment_cntr
			>> snd_death_cntr;
		//parse
		MPI_Send(world_height, world_width, ....);



		/*
		sendbuf.insert(sendbuf.end(), init_world[world_height - 1].begin(), init_world[world_height - 1].end());
		for (int i = 0; i < world_height; ++i) {
			sendbuf.insert(sendbuf.end(), init_world[i].begin(), init_world[i].end());
		}
		sendbuf.insert(sendbuf.end(), init_world[0].begin(), init_world[0].end());
		*/
		for (int id = 0; id < proc_cnt; id++) {
	        int top = get_border(id, proc_cnt, world_height) - 1;
			int bottom = get_border(id + 1, proc_cnt, world_height);
			if (bottom == 0) {
				bottom = world_height;
			}
			if (id != root) {
				MPI_Send(bottom - top + 1)
				for (int i = top; i <= bottom; ++i) {
					if (bottom == world_height) bottom = 0;
    				MPI_Bsend(&init_world[i][0], world_width, mpi_point_type, id, 1, MPI_COMM_WORLD);
				}
			} else {
				old_world.insert(old_world.end(), init_world[world_height - 1].begin(), init_world[world_height - 1].end());
				for (int i = 0; i < bottom; ++i) {
					old_world.insert(old_world.end(), init_world[i].begin(), init_world[i].end());
				}
			}
	    }
	} else {
		MPI_Recv(world_height, world_width, nut_recover_rate >> fst_excr_rate >> snd_excr_rate 
			>> nut_eat_rate >> fst_eat_rate >> snd_eat_rate 
			>> nut_segment_cntr >> fst_segment_cntr
			>> snd_death_cntr);
		int row_cnt;
		MPI_Recv(&row_cnt, 1, MPI_INT, root, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		for (int i = 0; i < row_cnt; ++i) {
			old_world.push_back(vector<point>(world_width));
			world.push_back(vector<point>(world_width));
		}
	    for (int i = 0; i < row_cnt; ++i) {
		    //vector<point> recvbuf(world_width);
		    //MPI_Recv(&recvbuf[0], size, mpi_point_type, proc_cnt - 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		    //old_world.insert(old_world.end(), recvbuf.begin(), recvbuf.end());
		    MPI_Recv(&old_world[i][0], world_width, mpi_point_type, root, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	    }
	}
    int border_top = get_border(proc_id, proc_cnt, world_height) - 1;
	int border_bottom = get_border(proc_id + 1, proc_cnt, world_height);
	if (border_bottom < border_top) {
		border_bottom = world_height;
	}
	for (int iter = 0; iter < iter_cnt; ++iter) {
		/*
		 * Calculate.
		 */
		for (int i = 1; i < border_bottom ; ++i) {
			for (int j = 0; j < world_width; ++j) {
				/*
				 * Eat.
				 */
				vector<int> nb_nut_cntr(9);
				for (int nb = 0; nb < 9; ++nb) {
					int x = (i + nb_x[nb] + world_height) % world_height;
					int y = (j + nb_y[nb] + world_width) % world_width;
					nb_nut_cntr[nb] = old_world[x][y].nut_cntr;
				}
				for (int cnt = 0; cnt < fst_cnt; cnt++) {
					if (nut_eat_rate <= nut_cntr) {
						nut_cntr -= nut_eat_rate;
						world[i][j].fst_cnt++;
					} else {
						bool alive = true;
						for (int nb = 0; nb < 9; ++nb) {
							int x = (i + nb_x[nb] + world_height) % world_height;
							int y = (j + nb_y[nb] + world_width) % world_width;
							if (nb_nut_cntr[nb] < nut_eat_rate / 9) {
								nb_nut_cntr[nb] -= nut_eat_rate / 9);
							}
						} else {
							alive = false;
							break;
						}
						if (alive) {

						}
					}	
				}
				
				/*
				 * Death.
				 */
				if (snd_death_cntr == old_world[i][j].snd_cntr) {
					world[i][j].fst_cnt = 0;
				}

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
		}
		/*
         * Check on stoppage condition.
         */
        int sum = world.size(), cmp = 0, alive_cells, differ, stop_cnd;
        spii::iterator old_it = old_world.begin();
        it = world.begin();
        for (; old_it != old_world.end() && it != world.end(); ++it, ++old_it) {
        	if(*it != *old_it) {
        		cmp++;
        	}
        }
        MPI_Reduce(&sum, &alive_cells, 1, MPI_INT, MPI_SUM, root, MPI_COMM_WORLD);
        MPI_Reduce(&cmp, &differ, 1, MPI_INT, MPI_SUM, root, MPI_COMM_WORLD);
		if (proc_id == root) {
			stop_cnd = (!alive_cells || !differ) ? 1 : 0;
			if (stop_cnd) {
				printf("Stoppage condition on iteration %d\n", iter);
			}
		}
		MPI_Bcast(&stop_cnd, 1, MPI_INT, root, MPI_COMM_WORLD);
		if (stop_cnd) {
			break;
		}
		
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
		int size = above_row.size();
		MPI_Send(&size, 1, MPI_INT, (proc_id - 1 + proc_cnt) % proc_cnt, 0, MPI_COMM_WORLD);
		MPI_Send(&above_row[0], size, mpi_point_type, (proc_id - 1 + proc_cnt) % proc_cnt, 1, MPI_COMM_WORLD);
		size = under_row.size();
		MPI_Send(&size, 1, MPI_INT, (proc_id + 1) % proc_cnt, 0, MPI_COMM_WORLD);
		MPI_Send(&under_row[0], size, mpi_point_type, (proc_id + 1) % proc_cnt, 1, MPI_COMM_WORLD);
		/*
		 * Receive.
		 */
		MPI_Recv(&size, 1, MPI_INT, (proc_id - 1 + proc_cnt) % proc_cnt,
				 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	    above_row.clear();
	    above_row.resize(size);
	    MPI_Recv(&above_row[0], size, mpi_point_type, (proc_id - 1 + proc_cnt) % proc_cnt,
	    		 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(&size, 1, MPI_INT, (proc_id + 1) % proc_cnt, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	    under_row.clear();
	    under_row.resize(size);
	    MPI_Recv(&under_row[0], size, mpi_point_type, (proc_id + 1) % proc_cnt, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		for (int i = 0; i < (int)above_row.size(); ++i) {
			world.insert(above_row[i]);
		}
		for (int i = 0; i < (int)under_row.size(); ++i) {
			world.insert(under_row[i]);
		}

		old_world.clear();
        swap(old_world, world);
	}
    sendbuf.clear();
    spii::iterator it = old_world.begin();
	for (; it != old_world.end(); ++it) {
		if (it->x >= border_top && it->x < border_bottom) {
			sendbuf.pb(mp(it->x, it->y));
		}
	}
    int scount = sendbuf.size();
    vi rcounts(proc_cnt);
    MPI_Gather(&scount, 1, MPI_INT, &rcounts[0], 1, MPI_INT, root, MPI_COMM_WORLD);
    for (int i = 1; i < proc_cnt; ++i) {
    	displs[i] = displs[i - 1] + rcounts[i - 1];
    }
    recvbuf.clear();
    recvbuf.resize(displs[proc_cnt - 1] + rcounts[proc_cnt - 1]);
    MPI_Gatherv(&sendbuf[0], scount, mpi_point_type, &recvbuf[0], &rcounts[0], 
    			&displs[0], mpi_point_type, root, MPI_COMM_WORLD);
	if (proc_id == root) {
		for (int i = 0; i < (int)recvbuf.size(); ++i) {
			world.insert(mp(recvbuf[i].x, recvbuf[i].y));
        }
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
	}
	MPI_Type_free(&mpi_point_type);
	MPI_Finalize();
	return 0;
}
