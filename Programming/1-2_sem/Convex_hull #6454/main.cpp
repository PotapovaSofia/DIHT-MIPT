#include "polygon.h"
#include "convex_hull.cpp"

int main()
{
	vector<pt> A;
	srand (time(NULL));
	for(int i = 0; i < 10; ++i)
	{
		A[i].y = rand() % 20;
		A[i].x = rand() % 20;
	}
	Polygon hull(A);
	hull.Print(hull);
	return 0;
}