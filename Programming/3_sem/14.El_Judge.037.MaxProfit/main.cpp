#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <vector>
#include <map>

using namespace std;

vector<vector<int> > table;
map< int, int >::iterator it;
vector <int> weight;

void rec(int k, int B, vector<int> &result) {
    if (table[k][B] == 0) {
        return;
    }
    if (table[k][B] != table[k-1][B]) {
        while(it->first > k) {
            --it;
        }
        ++result[it->second];
        rec(k - 1, B - weight[it->second], result);
    } else {
        rec(k - 1, B, result);
    }
}

int main() {

    int N, A, B, k = 0;
    cin >> N >> A >> B;

    map <int ,int> mmap;
    vector <int> price (N);
    //vector <int> weight (N);
    weight.resize(N);
    vector <int> quantity (N);

    for (int i = 0; i < N; ++i) {
        cin >> price[i] >> weight[i] >> quantity[i];
        mmap.insert(make_pair(k+1, i));
        k += quantity[i];
    }

    table.resize(k+1);

    for (int i = 0; i < table.size(); ++i) {
        table[i].resize(B+1);
    }
    for (int i = 0; i <= k; ++i) {
        table[i][0] = 0;
    }
    for (int i = 0; i <= B; ++i) {
        table[0][i] = 0;
    }

    int t = -1;
    for (int i = 1; i <= k; ++i) {
        if (mmap.find(i) != mmap.end()) {
            ++t;
        }
        for (int j = 1; j <= B; ++j) {
            if (j >= weight[t]) {
                table[i][j] = max(table[i-1][j], table[i-1][j-weight[t]] + price[t]);
            } else {
                table[i][j] = table[i-1][j];
            }
        }
    }

    if (table[k][B] < A) {
        cout << "-1\n";
        return 0;
    }
    cout << table[k][B] << "\n";
    vector<int> result (N, 0);
    it = mmap.end();
    it--;

    rec(k, B, result);
/*
    while (true) {
        if (table[k][B] == 0) {
            break;
        }
        //--k;
        if (table[k-1][B] != table[k][B]) {
            while(it->first > k) {
                --it;
            }
            ++result[it->second];
            B -= weight[it->second];
        }
        --k;
    }
*/
    for (int i = 0; i < result.size(); ++i) {
        cout << result[i] << "\n";
    }
}
