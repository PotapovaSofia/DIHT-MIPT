#ifndef SHAPE_H_INCLUDED
#define SHAPE_H_INCLUDED

#include "point.h"
#include "segment.h"

class shape
{
public:
	virtual double area() = 0;
	virtual bool pointOnBorder(point& _A) = 0;
	virtual bool pointOnShape(point& _A) = 0;
	virtual bool interSegShape(segment& _AB, point &_C) = 0;
};

#endif // SHAPE_H_INCLUDED