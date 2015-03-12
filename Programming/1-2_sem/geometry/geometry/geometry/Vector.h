#ifndef VEC_H_INCLUDED
#define VEC_H_INCLUDED

#include "segment.h"
#include <iostream>

class vec : public segment
{
public:
	vec(point _A = point(), point _B = point());
	vec(const vec& _AB);

	~vec();

	void convVec();
	void rVec(segment _AB);
	bool pointVec(point _C);
};

vec::vec(point _A, point _B): segment(_A, _B) { }

vec::vec(const vec& _AB): segment(_AB) { }

void vec::convVec()
{
	setSeg(getSegB(), getSegA());
}

void vec::rVec(segment _AB)
{
	point _B;
	_B = _AB.rVecPoint();
	point _A;
	setSeg(_A, _B);
}

bool vec::pointVec(point _C)
{
	if (vectorProduct(_C))
	{
		return true;
	}
}


#endif // VEC_H_INCLUDED
