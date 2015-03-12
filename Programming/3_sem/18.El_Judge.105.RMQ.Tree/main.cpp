//#include <iostream>

using namespace std;
#include<math.h>
#include <vector>
#include <stdio.h>

#define T float
#define min(a, b) (a) < (b) ? (a) : (b)
//template <class T = int>
class SparseTable {
    vector<int> log_for_length;
    vector<T> array;
    vector<vector<T> > sparse_table;
    //T (*func) (T, T);

public:
    SparseTable(const vector<T> & arr/*, T (*_func)(T , T )*/) {
        //func = _func;
        array = arr;
        preproc_log();
        building_sparse_table();
    }

    T request(int l, int r) {
        /*
        if (l > r) {
            exit(1);
        }*/
        if (l < 0) {
            l = 0;
        }
        if (r >= array.size()) {
            r = array.size() - 1;
        }
        int log = log_for_length[r - l + 1];
        return min(sparse_table[log][l], sparse_table[log][r - (1 << log) + 1]);
    }

private:

    void building_sparse_table() {
        int n = (int)log2(array.size());
        sparse_table.resize(n + 1);
        for (int i = 0; i < array.size(); ++i) {
            sparse_table[0].push_back(array[i]);
        }
        for (int k = 1; k <= n; ++k) {
            for (int i = 0; (i + (1 << k)) <= array.size(); ++i) {
                sparse_table[k].push_back(min(sparse_table[k - 1][i], sparse_table[k - 1][i + (1 << (k - 1))]));
            }
        }
    }

    void preproc_log() {
        log_for_length.reserve(array.size()+1);
        log_for_length.push_back(0); //для длины 0
        log_for_length.push_back(0); // для длины 1
        for (int i = 2; i <= array.size(); ++i) {
            log_for_length[i] = log_for_length[i/2] + 1;
        }
    }
};
/*
template <typename T>
T min_f (T f, T s) {
    if (f < s) {
        return f;
    } else {
        return s;
    }
}
*/
int main() {
    int N, M;
    float x, y, a;
    vector<float> answer;
    vector<float> array;
    //cin >> N;
    scanf("%d", &N);
    for (int i = 0; i < N; i++) {
        //cin >> a;
        scanf("%f", &a);
        array.push_back(a);
    }
    SparseTable st(array);
    //cin >> M;
    scanf("%d", &M);
    for (int i = 0; i < M; i++) {
        //cin >> x >> y;
        scanf("%f %f", &x, &y);
        a = st.request(x, y-1);
        answer.push_back(a);
    }

    for (int i= 0; i < answer.size(); i++) {
        printf("%f ", answer[i]);
    }
}