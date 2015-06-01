#include <vector> 
#include <cstdio>
#include <iostream>
#include <cassert>

#include "parallel_kd_tree.h"

int main() {
	assert(freopen("test.txt", "r", stdin));
	int n, k;
	std::cin >> n >> k;
	std::vector<Point> points(n);
	for(int i = 0; i < n; i++) {
		points[i].read_random_point(k);
	}
	KD_Tree kd(points, k);
	//kd.print_tree();
	for(int i = 0; i < 10; i++) {
		Point nearest = kd.find_nearest_neighbor(points[i]);
		int nearestd = points[i].distance(nearest);
		int rightd = KD_Tree::inf;
		Point rightpoint;
		for (int j = 0; j < n; j++) {
			 if (i != j) {
				int newd = points[i].distance(points[j]);
				if (newd != 0 && newd < rightd) {
					rightd = newd;
					rightpoint = points[j];
				}
			}
		}
		assert(rightd == nearestd);
	}
	return 0;
}