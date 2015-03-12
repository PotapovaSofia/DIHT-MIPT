#include "polygon.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <time.h>

using namespace std;

Polygon::Polygon()
{
	_data = (pt* )malloc(sizeof(pt));
}

Polygon::Polygon(int size)
{
	srand (time(NULL));
	//_data = (pt* )malloc(sizeof(pt)*size);
	for(int i = 0; i < size; ++i)
	{
		_data[i].y = rand() % size*2;
		_data[i].x = rand() % size*2;
	}
}

Polygon::~Polygon()
{
	~_data();
}

bool compare (pt a, pt b) 
{
	return (a.x < b.x) || (a.x == b.x && a.y < b.y);
}

bool clockwise (pt a, pt b, pt c) 
{
	return ( a.x*(b.y - c.y) + b.x*(c.y - a.y) + c.x*(a.y - b.y) ) < 0;
}

bool counter_clockwise (pt a, pt b, pt c) 
{
	return ( a.x*(b.y - c.y) + b.x*(c.y - a.y) + c.x*(a.y - b.y) ) > 0;
}

Polygon::Polygon(vector<pt> &data)
{
	if (data.size() < 3)
		cout << "Bad size of your data" << endl;
	sort(data.begin(), data.end(), &compare);
	//qsort(data, data.size(), sizeof(pt), compare);
	pt p1 = data[0],  p2 = data.back();
	vector<pt> up, down;
	up.push_back (p1);
	down.push_back (p1);
	for (int i = 1; i < data.size(); ++i) 
	{
		if (i == data.size() - 1 || clockwise( p1, data[i], p2 ))
		{
			while (up.size() >= 2 && !clockwise( up[up.size() - 2], up[up.size() - 1], data[i] ))
				up.pop_back();
			up.push_back (data[i]);
		}
		if (i == data.size() - 1 || counter_clockwise (p1, data[i], p2)) 
		{
			while (down.size() >= 2 && !counter_clockwise (down[down.size() - 2], down[down.size() - 1], data[i] ))
				down.pop_back();
			down.push_back (data[i]);
		}
	}
	data.clear();
	for (int i = 0; i < up.size(); ++i)
		data.push_back (up[i]);
	for (int i = down.size() - 2; i > 0; --i)
		data.push_back (down[i]);
	_data = data;
}

void Polygon::Print(Polygon a)
{
	for(int i = 0; i < a._data.size(); ++i)
	{
		std::cout << "X"<< i << ": " << a._data[i].x << "\n";
		std::cout << "Y"<< i << ": " << a._data[i].y << "\n";
		std::cout << "\n";
	}
}