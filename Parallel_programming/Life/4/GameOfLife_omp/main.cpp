#include <omp.h>
#include <iostream>
#include <vector>
#include <array>
#include <string.h>
#include <unistd.h>

using namespace std;

volatile int status = 0; // Execution status
volatile int force_stop = 0;  // Flag for quick stopping threads
volatile int iterations_left = 0; // Counter of remaining iterations
volatile size_t unfinished_threads_counter; // Counter of not finishing iteration threads
vector<int> start_iteration_flags;

size_t n = 0, m = 0; // Field size
size_t threads_num = 0;
array<vector< vector<short> >, 2> fields;
volatile int curr_field = 0; // Current field pointer
array<pair<short, short>, 8> neighbours_indices = {
        {{1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1}, {0, 1}, {1, 1}}
};

void worker(size_t num, size_t top_border, size_t bot_border) {
    // Iteration cycle
    while(true) {
        while (force_stop == 0 && start_iteration_flags[num] == 0) {
            #pragma omp taskyield
        }

        if (force_stop == 1)
            break;
        start_iteration_flags[num]--;
        int next_field = (curr_field + 1) % 2;

        for (size_t i = top_border; i < bot_border; i++) {
            for (size_t j = 0; j < m; j++) {
                int alive_neighbours = 0;
                for (int neighbour_num = 0; neighbour_num < 8; neighbour_num++) {
                    size_t k = (i + neighbours_indices[neighbour_num].first + n) % n;
                    size_t l = (j + neighbours_indices[neighbour_num].second + m) % m;
                    if ((k != i || l != j) && fields[curr_field][k][l]) {
                        alive_neighbours++;
                    }
                }
                if (fields[curr_field][i][j] == 0) {
                    fields[next_field][i][j] = (alive_neighbours == 3);
                }
                else {
                    fields[next_field][i][j] = (alive_neighbours == 2 || alive_neighbours == 3);
                }
            }
        }

        #pragma omp critical
        {
            unfinished_threads_counter--;
            if (unfinished_threads_counter == 0) {
                // Mini master
                unfinished_threads_counter = threads_num;
                curr_field = next_field;
                if (iterations_left > 0)
                    iterations_left--;
                if (iterations_left <= 0) {
                    iterations_left = 0;
                    status = 1;
                } else {
                    for (int i = 0; i < threads_num; i++) {
                        start_iteration_flags[i]++;
                    }
                }
            }
        }

    }
}

int read_field_from_file() {
    FILE* file;
    string path;
    int ex_status = 0;

    // Open file
    cout << "Specify path to file:" << endl;
    cin >> path;
    file = fopen(path.c_str(), "r");
    if (file == NULL) {
        cout << "Can't open file: " << strerror(errno) << endl;
        return -1;
    }

    // Count rows and columns
    size_t tmp = 0; int ch;
    while((ch = fgetc(file)) != EOF) {
        if (ch == ',') tmp++;
        if (ch == '\n') {
            tmp++;
            if (n == 0) {
                m = tmp;
            } else if (tmp != m) {
                cout << "Error reading from file: wrong format" << endl;
                ex_status = -1;
                break;
            }
            tmp = 0;
            n++;
        }
    }
    if (ex_status != 0) {
        fclose(file);
        return ex_status;
    } else {
        rewind(file);
    }

    // Read lines
    fields[curr_field].resize(n);
    fields[(curr_field + 1) % 2].resize(n);
    for (int i = 0; i < n; i++) {
        fields[curr_field][i].resize(m);
        fields[(curr_field + 1) % 2][i].resize(m);
        for (int j = 0; j < m; j++) {
            while ((ch = fgetc(file)) != ',' && ch != '\n' && ch != EOF) {
                if (ch == ' ') {
                    continue;
                }
                if (ch == '0' || ch == '1') {
                    fields[curr_field][i][j] = (short) (ch - '0');
                }
                else {
                    cout << "Error reading file: wrong format" << endl;
                    ex_status = -1;
                    break;
                }
            }
            if (ex_status != 0) break;
        }
        if (ex_status != 0) break;
    }
    if (ex_status == 0) {
        cout << "Field read: " << n << "x" << m << endl;
    }
    fclose(file);
    return ex_status;
}

void generate_random_field() {
    srand((unsigned) time(NULL));
    cout << "Field will be generated in random way" << endl;
    do {
        cout << "Specify number of rows: ";
        cin >> n;
    } while (n == 0);
    do {
        cout << "Specify number of columns: ";
        cin >> m;
    } while (m == 0);
    // Generate
    fields[curr_field].resize(n);
    fields[(curr_field + 1) % 2].resize(n);
    for (int i = 0; i < n; i++) {
        fields[curr_field][i].resize(m);
        fields[(curr_field + 1) % 2][i].resize(m);
        for (int j = 0; j < m; j++) {
            fields[curr_field][i][j] = (short) (rand() % 2);
        }
    }
    cout << "Field generated: " << n << "x" << m << endl;
}

int main() {
    omp_set_nested(1);
    // Main cycle
    string cmd;
    double  dif, start, end;


    #pragma omp parallel num_threads(2)
    {
        #pragma omp single
        {
            while (true) {
                cin >> cmd;
                if (cmd == "START") {
                    // Check whether threads are already created
                    if (status > 0) {
                        cout << "Already started" << endl;
                        continue;
                    }
                    // Create or read field
                    while (true) {
                        cout << "Read start config from file? [y/n] ";
                        cin >> cmd;
                        if (cmd == "y" or cmd == "n")
                            break;
                    }
                    if (cmd == "y") {
                        int stat;
                        do {
                            stat = read_field_from_file();
                        } while (stat);
                    } else {
                        generate_random_field();
                    }
                    // Create threads
                    do {
                        cout << "Specify number of threads: " << endl;;
                        cin >> threads_num;
                    } while (threads_num == 0);
                    cout << "threads_num: " << threads_num << endl;
                    start_iteration_flags.reserve(threads_num);
                    unfinished_threads_counter = threads_num;
                    start = omp_get_wtime( ); //start the timer
                    
                    #pragma omp task
                    #pragma omp parallel num_threads(threads_num)
                    {
                        #pragma omp single
                        {
                            size_t remainder = n % threads_num;
                            size_t prev_top_border = 0;
                            for (size_t i = 0; i < threads_num; i++) {
                                size_t bot_border = prev_top_border + (n / threads_num);
                                if (remainder > 0) {
                                    bot_border++;
                                    remainder--;
                                }
                                start_iteration_flags[i] = 0;
                                #pragma omp task
                                worker(i, prev_top_border, bot_border);
                                prev_top_border = bot_border;
                            }
                        }
                    }
                    status = 1;
                    cout << "START" << endl;
                }
                else if (cmd == "STATUS") {
                    if (status == 0) {
                        cout << "Field hasn't been initialized" << endl;
                    }
                    else if (status == 1) {
                        for (int i = 0; i < n; i++) {
                            for (int j = 0; j < m; j++) {
                                if (fields[curr_field][i][j])
                                    cout << "O";
                                else
                                    cout << ".";
                            }
                            cout << endl;
                        }
                    }
                    else {
                        cout << "Threads are running now" << endl;
                        cout << "Iterations left: " << iterations_left << endl;
                    }
                }
                else if (cmd == "RUN") {
                    if (status == 0) {
                        cout << "Field hasn't been initialized" << endl;
                    }
                    else if (status == 1) {
                        int tmp;
                        do {
                            cout << "Specify number of iterations: " << endl;;
                            cin >> tmp;
                        } while (tmp <= 0);
                        cout << "iter: " << tmp << endl;
                        status = 2;
                        iterations_left = tmp;
                        for (int i = 0; i < threads_num; i++) {
                            start_iteration_flags[i]++;
                        }
                    }
                    else {
                        cout << "Threads already running" << endl;
                        exit(0);
                    }
                    cout << "RUN" << endl;
                }
                else if (cmd == "AWAIT") {
                    cout << "try to await" << endl;
                    if (status == 0) {
                        cout << "Field hasn't been initialized" << endl;
                    }
                    else
                        while (status != 1) {}
                    end = omp_get_wtime();// end the timer
                    cout << "AWAIT" << endl;
                }
                else if (cmd == "STOP") {
                    if (status == 0) {
                        cout << "Field hasn't been initialized" << endl;
                    }
                    else if (status == 1) {
                        cout << "Threads aren't running now" << endl;
                    } else {
                        iterations_left = 0;
                        status = 1;
                    }
                }
                else if (cmd == "QUIT") {
                    if (status > 0) {
                        force_stop = 1; // Signal to threads to stop
                        #pragma omp taskwait
                    }
                    cout << "QUIT" << endl;
                    dif = end - start; // stores the difference in dif
                    printf("the time of dif is %f", dif);
                    
                    
                    break;
                }
                else {
                    cout << "Unknown command: " << cmd << endl;
                    cout << "Known commands:" << endl;
                    cout << "\tSTART" << endl;
                    cout << "\tSTATUS" << endl;
                    cout << "\tRUN" << endl;
                    cout << "\tSTOP" << endl;
                    cout << "\tQUIT" << endl;
                    cout << "Try again..." << endl;
                }
            }
        }
    }
    return 0;
}