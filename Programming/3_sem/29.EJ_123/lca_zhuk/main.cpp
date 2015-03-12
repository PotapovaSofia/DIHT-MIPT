//#include <bits/stdc++.h>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

const int M = 400001;
const int A = 19;

vector<int> g[M];
int p[M][A];
int n;
int tin[M], tout[M], tim = 0;
bool used[M];

void read() {
    cin >> n;
    for (int i = 0, x, y; i < n; ++i) {
        cin >> x >> y;
        g[x].push_back(y);
        g[y].push_back(x);
    }
}

void dfs(int v, int from) {
    tin[v] = tim++;
    used[v] = true;

    p[v][0] = from;
    for (int i = 1; i < A; ++i)
        p[v][i] = p[p[v][i - 1]][i - 1];

    for (int i = 0; i < g[v].size(); i++) {
        if(!used[g[v][i]])
            dfs(g[v][i], v);
    }
    tout[v] = tim++;
}

bool isAncestor(int anc, int x) {
    return tin[anc] <= tin[x] && tout[x] <= tout[anc];
}

int lca(int x, int y) {
    if (isAncestor(x, y))
        return x;

    for (int i = A - 1; i >= 0; --i)
        if (!isAncestor(p[x][i], y))
            x = p[x][i];

    return p[x][0];
}

void kill() {
    int m;
    cin >> m;
    for (int i = 0, x, y; i < m; ++i) {
        cin >> x >> y;
        cout << lca(x, y) << endl;
    }
}

int main() {
    //ios_base::sync_with_stdio(false);

    read();
    dfs(0, 0);
    kill();

    return 0;
}