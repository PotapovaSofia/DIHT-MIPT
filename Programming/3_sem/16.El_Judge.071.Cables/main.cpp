#include <iostream>
#include <vector>

using namespace std;

int main() {
    int N, K;
    long long c, left = 0, right = 0, mid, result = 0;
    cin >> N >> K;
    vector<long long> lengths(N);
    for (int i = 0; i < N; ++i) {
        cin >> lengths[i];
        right += lengths[i];
    }
    ++right;

    while (right != left + 1) {
        c = 0;
        mid = (right + left) / 2;
        for (int i = 0; i < N; ++i) {
            c += lengths[i] / mid;
        }
        if (c < K) {
            right = mid;
        } else {
            left = mid;
            result = result < mid ? mid : result;
        }
    }
    cout << result;
    return 0;
}