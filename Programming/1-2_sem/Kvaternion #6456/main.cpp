#include "kvater.h"
#include "kvater.cpp"

int main()
{
	kvaternion a();
	kvaternion b(1.5);
	kvaternion c(3.0, 2.1, 2.2, 1.15);
	kvaternion d;
	d = a + b;
	print(d);
	d += c;
	if (d == a)
		printf("a = d");
	double x,y,z;
	b = conjugate(c);
	x = module(a);
	y = dot(c,d);
	kvaternion e = cross(c, d);
	print(e);
}