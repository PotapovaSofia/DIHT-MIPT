#include <bitset>
#include <vector>
#include <climits>
#include <iostream>
#include <cmath>

std::vector<int> bit_reverse_copy(std::vector<int> a) {
    int n = a.size();
    int p = log2(n);
    std::vector<int> b(n);
    for (int k = 0; k < n; k++) {
        std::cout << k << std::endl;
        std::bitset<sizeof(k) * CHAR_BIT> x = k;
        for (int i = 0; i < x.size(); i++) {
            std:: cout << x[i];
        }
        std::cout << std::endl;
        std::bitset<sizeof(k) * CHAR_BIT> y;
        for (int i = 0; i < p; i++) {
            y[i] = x[p - i - 1];
        }
        for (int i = 0; i < y.size(); i++) {
            std:: cout << y[i];
        }
        std::cout << std::endl;
        std::cout << y.to_ulong() << std:: endl;
        b[y.to_ulong()] = a[k];
    }
    for (int i = 0; i < b.size(); i++) {
        std:: cout << b[i] << " ";
    }
    std::cout << std::endl;
    return b;
}

int main() {
    std::vector<int> vct = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    std::vector<int> v = bit_reverse_copy(vct);

    return 0;
}