#include <iostream>
#include <vector>
#include <limits.h>

using namespace std;

int maxFlow(vector<vector <int> > &array, int s, int t) {
    int n = array.size();

    vector<int> h(n); //высота вершины
    h[s] = n - 1;

    vector<int> maxh(n);
    vector<int> e(n); //избыточный поток
    vector<vector <int> > f(n, vector<int>(n)); //предпоток

    for (int i = 0; i < n; ++i) {
        f[s][i] = array[s][i];
        f[i][s] = -f[s][i];
        e[i] = array[s][i];
    }

    for (int sz = 0;;) {
        if (sz == 0) {
            for (int i = 0; i < n; ++i) {
                if (i != s && i != t && e[i] > 0) {
                    if (sz != 0 && h[i] > h[maxh[0]])
                        sz = 0;
                    maxh[sz++] = i;
                }
            }
        }
        if (sz == 0)
            break;
        while (sz != 0) {
            int i = maxh[sz - 1];
            bool pushed = false;
            for (int j = 0; j < n && e[i] != 0; ++j) {
                if (h[i] == h[j] + 1 && array[i][j] - f[i][j] > 0) {
                    int df = min(array[i][j] - f[i][j], e[i]);
                    f[i][j] += df;
                    f[j][i] -= df;
                    e[i] -= df;
                    e[j] += df;
                    if (e[i] == 0)
                        --sz;
                    pushed = true;
                }
            }
            if (!pushed) {
                h[i] = INT_MAX;
                for (int j = 0; j < n; ++j) {
                    if (h[i] > h[j] + 1 && array[i][j] - f[i][j] > 0) {
                        h[i] = h[j] + 1;
                    }
                }
                if (h[i] > h[maxh[0]]) {
                    sz = 0;
                    break;
                }
            }
        }
    }
    int flow = 0;
    for (int i = 0; i < n; i++) {
        flow += f[s][i];
    }
    return flow;
}

int main() {
    vector<vector<int> > array = { { 0, 3, 2 }, { 0, 0, 2 }, { 0, 0, 0 } };
    //vector<vector<int> > array = {{0, 1, 0, 4, 0, 0}, {0, 0, 2, 2, 3, 0}, {0, 0, 0, 0, 0, 2}, {0, 0, 3, 0, 0, 0},
      //            {0, 0, 0, 0, 0, 1}, {0, 0, 0, 0, 0, 0}};
    cout << maxFlow(array, 0, 5);
    return 0;
}