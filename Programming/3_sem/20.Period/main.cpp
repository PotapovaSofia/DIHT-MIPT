#include <iostream>
#include <string>
#include <vector>
#include <cstdio>

using namespace std;

int main() {
    string s;
    cin >> s;
    vector <int> p(s.length());
    int k = 0;
    for (int i = 1; i < s.length(); i++) {
        while(k > 0 && s[i] != s[k]) {
            k = p[k - 1];
        }
        if (s[i] == s[k]) {
            k++;
        }
        p[i] = k;
    }
    int ans = s.length() - p[s.length() - 1];
    if (s.length() % ans == 0) {
        cout << ans << endl;
    } else {
        cout << "No" << endl;
    }
    return 0;
}