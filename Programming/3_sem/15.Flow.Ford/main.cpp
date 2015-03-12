#include <iostream>
#include <limits.h>
#include <vector>

using namespace std;

int findPath(vector<vector<int> > &array, vector<bool> isVisited, int u, int t, int f) {
    if (u == t) {
        return f;
    }
    isVisited[u] = true;
    for (int v = 0; v < isVisited.size(); v++) {
        if (!isVisited[v] && array[u][v] > 0) {
            int df = findPath(array, isVisited, v, t, min(f, array[u][v]));
            if (df > 0) {
                array[u][v] -= df;
                array[v][u] += df;
                return df;
            }
        }
    }
    return 0;
}

int maxFlow(vector<vector<int> > array, int s, int t) {
    for (int flow = 0;;) {
        vector<bool> vct(array.size());
        int df = findPath(array, vct, s, t, INT_MAX);
        if (df == 0) {
            return flow;
        }
        flow += df;
    }
}

int main() {
    vector<vector<int> > array = { { 0, 3, 2 }, { 0, 0, 2 }, { 0, 0, 0 } };
    //vector<vector<int> > array = {{0, 1, 0, 4, 0, 0}, {0, 0, 2, 2, 3, 0}, {0, 0, 0, 0, 0, 2}, {0, 0, 3, 0, 0, 0},
      //      {0, 0, 0, 0, 0, 1}, {0, 0, 0, 0, 0, 0}};
    cout << maxFlow(array, 0, 5); //answer == 4
    return 0;
}