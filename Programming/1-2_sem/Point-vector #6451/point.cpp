#include "point.h"

Point::Point()
	: _x(0.0), _y(0.0)
{}

Point::Point(int const x, int const y)
	: _x(x), _y(y)
{}

Point::Point(Point const& a)
	: _x(a._x), _y(a._y)
{}


Point Point::operator+(Point const& other) const
{
	Point p;
	p._x = _x + other._x;
	p._y = _y + other._y;
	return p;
}

Point Point::operator-(Point const& other) const
{
	Point p;
	p._x = _x - other._x;
	p._y = _y + other._y;
	return p;
}

Point Point::operator = (Point const& a) const
{
	Point b;
	b._x == _x;
	b._y == _y;
	return b;
}

bool Point::operator == (Point const& a) const
{
	if ( (a._x == _x) && (a._y == _y) )
		return true;
	return false;
}