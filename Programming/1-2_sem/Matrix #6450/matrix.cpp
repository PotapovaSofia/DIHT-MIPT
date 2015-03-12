#include <stdio.h>
#include <iostream>
#include "matrix.h"

using namespace std;

void main()
{
	int d, t;
	Matrix<int> a(3,3);
	Matrix<int> b(3,3);
	Matrix<int> c(3,3);
	cout << "First matrix 3x3: " << endl;
	//for(int i = 0; i < 3; ++i)
	//	for(int j = 0; j < 3; ++j)
	//		cin >> a.data[i][j];
	cout << "Second matrix 3x3: " << endl;
	//for(int i = 0; i < 3; ++i)
	//	for(int j = 0; j < 3; ++j)
	//		cin >> b.data[i][j];
	c = a + b;
	d = det(c);
	t = track(c);
	trans(a);
	inverse(a);
}
