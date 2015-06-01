#include <iostream>
#include <vector>
#include "FFT.h"

int main() {
	int n, m, x;
	std::vector<int> a;
	std::vector<int> b;
	std::cin >> n >> m;
	for (int i = 0; i < n; i++) {
		std::cin >> x;
		a.push_back(x);
	}
	for (int i = 0; i < m; i++) {
		std::cin >> x;
		b.push_back(x);
	}
	
	std::vector<int> c = multiply(a, b);
	for (int i = 0; i < c.size(); i++) {
		std::cout << c[i] << " ";
	}
	std::cout << std::endl;
	return 0;
}

