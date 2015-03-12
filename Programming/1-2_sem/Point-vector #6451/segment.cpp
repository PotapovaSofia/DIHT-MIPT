#include "segment.h"

Segment::Segment()
	: _p0(), _p1()
{}

Segment::Segment(int const x0,
          int const y0, 
          int const x1, 
          int const y1)
{
	Point p0(x0,y0);
	_p0 = p0;
	Point p1(x1,y1);
	_p1 = p1;
}

Segment::Segment(Point const& p0, 
			Point const& p1)
	: _p0(p0), _p1(p1)
{}

bool Segment::intersect(Segment const& s1, 
                        Segment const& s2,
                        Point& ptOfIntersect)
{
	double k1 = (s1._p0._y - s1._p1._y) / (s1._p0._x - s1._p1._x); // y = kx + b
	double b1 = s1._p1._y - k1 * s1._p1._x;
	double k2 = (s2._p0._y - s2._p1._y) / (s2._p0._x - s2._p1._x);
	double b2 = s2._p1._y - k2 * s2._p1._x;
	if ( (ptOfIntersect._y == ptOfIntersect._x * k1 + b1)
		&& (ptOfIntersect._y == ptOfIntersect._x * k2 + b2)
		&& isPointOnSegment(ptOfIntersect, s1)
		&& isPointOnSegment(ptOfIntersect, s1) )
		return true;
	return false;
}

bool Segment::isPointOnSegment(const Point& a, 
                             const Segment& s1)
{
	if (s1._p0 == s1._p1)
		if (s1._p0 == a)
			return true;
		else
			return false;
	double cross = (a._x - s1._p0._x)*(s1._p1._y - s1._p0._y) - (a._y - s1._p0._y)*(s1._p1._x - s1._p0._x); // (x-x1)(y2-y1)-(y-y1)(x2-x1) = 0
	bool b1 = ( (s1._p0._x < a._x) && (a._x < s1._p1._x) ); // x1 < x < x2
	bool b2 = ( (s1._p1._x < a._x) && (a._x < s1._p0._x) ); // x2 < x < x1
	if ( (cross == 0.0) && ( b1 || b2 ) ) // cross product(p0_a, p0_p1) = 0;
		return true;
	return false;
}

int Segment::rotateSegmentAroundPoint(const Point& a, 
                                    const Segment& s1)
{
	double k = (s1._p0._y - s1._p1._y) / (s1._p0._x - s1._p1._x); // y = kx + b
	double b = s1._p1._y - k * s1._p1._x;
	double result = a._y - k * a._x + b;
	if (result > 0)
		return 1;
	if (result = 0)
		return 0;
	return -1;
}