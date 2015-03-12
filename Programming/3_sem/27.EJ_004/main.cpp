#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

int main() {
    int N, m, s, sum_mass = 0, result = 0;
    cin >> N;
    vector<pair<int,int> > athlets;
    for (int i = 0; i < N; ++i) {
        cin >> m >> s;
        athlets.push_back(make_pair(m, s));
    }

    sort(athlets.begin(), athlets.end());

    for (vector<pair<int, int> >::iterator it = athlets.begin(); it != athlets.end(); ++it) {
        if (it->second >= sum_mass) {
            result++;
            sum_mass += it->first;
        }
    }
    cout << result << endl;
    return 0;
}