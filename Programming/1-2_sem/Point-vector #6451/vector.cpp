#include "vector.h"
#include "point.h"
#include "segment.h"

Vector::Vector()
	: _pt0(), _coords()
{}

Vector::Vector(Vector  const&a)
	: _pt0(a._pt0), _coords(a._coords)
{}

Vector::Vector(Segment const& a)
	: _pt0(a._p0), _coords(a._p1)
{}

Vector Vector::inverse(Vector const& a)
{
	Vector b;
	b._pt0 = b._coords;
	b._coords = b._pt0;
	return b;
}

