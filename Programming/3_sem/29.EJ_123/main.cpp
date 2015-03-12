#define _CRT_SECURE_NO_WARNINGS
#include <vector>
#include <iostream>

using namespace std;

typedef vector < vector<int> > graph;
typedef vector<int>::const_iterator const_graph_iter;

vector<int> hight, list, start, tree;
vector<char> used;
int n;

void dfs(const graph &g, int v, int h = 1) {
    used[v] = true;
    hight[v] = h;
    list.push_back(v);
    for (const_graph_iter i = g[v].begin(); i != g[v].end(); ++i)
        if (!used[*i]) {
            dfs(g, *i, h + 1);
            list.push_back(v);
        }
}

void build (int i, int l, int r) {
    if (l == r)
        tree[i] = list[l];
    else {
        int m = (l + r) >> 1;
        build(2*i, l, m);
        build(2*i+1, m+1, r);
        if (hight[tree[i + i]] < hight[tree[2*i + 1]])
            tree[i] = tree[i + i];
        else
            tree[i] = tree[2*i + 1];
    }
}

int get_min(int i, int sl, int sr, int l, int r) {
    if (sl == l && sr == r)
        return tree[i];
    int sm = (sl + sr) >> 1;
    if (r <= sm)
        return get_min(2*i, sl, sm, l, r);
    if (l > sm)
        return get_min(2*i + 1, sm + 1, sr, l, r);
    int ans1 = get_min(2*i, sl, sm, l, sm);
    int ans2 = get_min(2*i + 1, sm + 1, sr, sm + 1, r);
    return hight[ans1] < hight[ans2] ? ans1 : ans2;
}

int lca(int a, int b) {
    int left = start[a],
            right = start[b];
    if (left > right)  swap(left, right);
    return get_min(1, 0, list.size() - 1, left, right);
}

int main() {
    /*
    int m;
    scanf("%d", &n);

    for (int i = 0; i < n; ++i) {
        int x, y;
        scanf("%d%d", &x, &y);
        //g[x].push_back(y);
        g[y].push_back(x);
    }
    */
    graph g(n + 1);

    int n = 400000;

    for (int i = 0; i < n; ++i) {
        //int x = rand() % 400000, y = rand() % 400000;
        //scanf("%d%d", &x, &y);
        //g[x].push_back(y);
        //g[y].push_back(x);
        g[i].push_back(i+1);
    }


    //int n = g.size();
    hight.resize(n);
    list.reserve(n * 2);
    used.assign(n, 0);

    dfs(g, 0);

    int mm = list.size();
    tree.assign(list.size() * 4 + 1, -1);

    build(1, 0, mm - 1);

    start.assign(n, -1);
    for (int i = 0; i < mm; ++i) {
        int v = list[i];
        if (start[v] == -1)
            start[v] = i;
    }

/*
    scanf("%d", &m);
    for (int i = 0; i < m; ++i) {
        int v1, v2;
        scanf("%d%d", &v1, &v2);
        int v = lca(v1, v2);
        printf("%d\n", v);
    }
    */
    return 0;
}