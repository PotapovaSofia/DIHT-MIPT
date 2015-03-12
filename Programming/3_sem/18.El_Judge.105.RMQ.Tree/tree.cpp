#include <vector>
#include <stdio.h>
#include <iostream>
#include <cmath>
#include <limits.h>

#define T float
#define min(a, b) (a) < (b) ? (a) : (b)

using namespace std;

class SegmentTree {
    vector<T> tree;
    vector<T> array;
    T maxT;
public :
    SegmentTree(const vector<T> & arr, T max) {
        array = arr;
        maxT = max;
        if (array.size() != (1 << (int)log2(arr.size()))) {
            while (array.size() != (2 << (int)log2(arr.size()))) {
                array.push_back(maxT);
            }
        }
        tree.resize(array.size() * 2 - 1);
        building(0, 0 , array.size());
    }

    T request(int l, int r) {
        return (this->find(0, 0, array.size() - 1, l, r));
    }

private:
    void building(int i, int l, int r) {
        if (l == r) {
            return;
        }
        if (r - l == 1) {
            tree[i] = array[l];
        } else {
            int mid = (l + r) / 2;
            building(2 * i + 1, l, mid);
            building(2 * i + 2, mid, r);
            tree[i] = min(tree[2 * i + 1], tree[2 * i + 2]);
        }
    }

    T find (int i, int tree_l, int tree_r, int l, int r) {
        if (l > r)
            return maxT;
        if (l == tree_l && r == tree_r)
            return tree[i];
        int mid = (tree_l + tree_r) / 2;
        return min(find(i*2 + 1, tree_l, mid, l, min(r,mid)), find(i*2+2, mid + 1, tree_r, max(l, mid+1), r));
    }
};

int main() {
    int N, M;
    float x, y, a;
    vector<float> answer;
    vector<float> array;
    //cin >> N;
    scanf("%d", &N);
    float max = INT_MIN;
    for (int i = 0; i < N; i++) {
        //cin >> a;
        scanf("%f", &a);
        array.push_back(a);
        if (a > max) max = a;
    }
    SegmentTree st(array, max + 1);
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