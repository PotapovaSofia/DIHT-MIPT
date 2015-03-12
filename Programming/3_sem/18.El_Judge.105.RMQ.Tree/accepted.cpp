#include <iostream>
#include <vector>
#include <limits.h>

using namespace std;

void building( int i, int l, int r, vector<float>& tree, vector<float>& array) {
    if (l == r) {
        tree[i] = array[l];
    } else {
        int mid = (r + l) / 2;
        building(2 *i + 1, l, mid, tree, array);
        building(2 * i + 2, mid+1, r, tree, array);
        tree[i] = min(tree[2 * i + 1], tree[2 * i + 2]);
    }
}

float request(int i, int tree_l, int tree_r, int l, int r, vector<float>& tree) {
    if (l > r) {
        return INT_MAX;
    }
    if (l == tree_l && r == tree_r) {
        return tree[i];
    }
    int mid = (tree_r + tree_l) / 2;
    return min(request(2 * i + 1, tree_l, mid, l, min(r, mid), tree),
            request(2 * i + 2, mid+1, tree_r, max(l,mid+1), r, tree));
}

int main() {
    int N, M, x, y;
    float a;
    scanf("%d", &N);
    vector<float> array;
    array.reserve(N);
    vector<float> tree(4*N+1, INT_MAX);
    for (int i = 0; i < N; ++i) {
        scanf("%f", &a);
        array.push_back(a);
    }
    building(0, 0, N-1, tree, array);
    scanf("%d", &M);
    for(int i = 0; i < M; ++i) {
        scanf("%d%d", &x, &y);
        a = request(0, 0, N-1, x, y-1, tree);
        printf("%f\n", a);
    }
    return 0;
}