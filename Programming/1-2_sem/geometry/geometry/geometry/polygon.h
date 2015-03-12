#ifndef POLYGON_H_INCLUDED
#define POLYGON_H_INCLUDED

#include <iostream>
#include <cstdlib>
#include <vector>
#include "shape.h"
#include "segment.h"

class polygon : public shape {
public:
	polygon(int _psize = 0);
	polygon(std::vector <point> _ppoints);
	polygon(const polygon& other);

	int psize() const;

	bool pointOnBorder(point& _A);
	bool interSegShape(segment& _AB, point& _C);
	virtual point& operator [] (int index);

protected:
	std::vector <point> ppoints;
};

polygon::polygon(int _psize) {
	point p;
	p.x = 0;
	p.y = 0;
	for (int i = 0; i < _psize; ++i) {
		ppoints.push_back(p);
	}
}

polygon::polygon(std::vector <point> _ppoints) {
	ppoints.resize(_ppoints.size());
	for (int i = 0; i < _ppoints.size(); ++i) {
		ppoints[i] = _ppoints[i];
	}
}

polygon::polygon(const polygon& other) {
	ppoints.resize(other.ppoints.size());
	for (int i = 0; i < other.ppoints.size(); ++i) {
		ppoints[i] = other.ppoints[i];
	}
}

int polygon::psize() const {
	return ppoints.size();
}

bool polygon::pointOnBorder(point& _A) {
	for (int i = 1; i < psize(); ++i)
	{
		segment _AB((*(const_cast<polygon*>(this)))[i], (*(const_cast<polygon*>(this)))[i - 1]);
		if (_AB.pointOnSeg(_A))
		{
			return true;
		}
	}
	segment _AB((*(const_cast<polygon*>(this)))[0], (*(const_cast<polygon*>(this)))[psize() - 1]);
	if (_AB.pointOnSeg(_A))
	{
		return true;
	}
	return false;
}

bool polygon::interSegShape(segment& _AB, point& _C)
{
	for (int i = 1; i < psize(); ++i)
	{
		segment _CD((*(const_cast<polygon*>(this)))[i], (*(const_cast<polygon*>(this)))[i - 1]);
		if (_CD.intersectSeg(_AB, _C))
		{
			return true;
		}
	}
	segment _CD((*(const_cast<polygon*>(this)))[psize() - 1], (*(const_cast<polygon*>(this)))[0]);
	if (_CD.intersectSeg(_AB, _C))
	{
		return true;
	}
	else
	{
		return false;
	}
}

point& polygon::operator [] (int index)
{
	if ((index < 0) || (index >= psize()))
	{
		std::cout << "Error index" << std::endl;
		exit(1);
	}
	else
	{
		return ppoints[index];
	}
}
#endif // POLYGON_H_INCLUDED