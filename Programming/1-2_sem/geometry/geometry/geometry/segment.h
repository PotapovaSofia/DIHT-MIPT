#include <iostream>
#include "point.h"

#ifndef SEGMENT_INCLUDED
#define SEGMENT_INCLUDED

class segment  {
public:
	segment() { }
	segment(point&, point&);
	bool pointOnSeg(point&);
	bool intersectSeg(segment);
	bool intersectSeg(segment, point&);
	double vectorProduct(point _C);
	double vectorProduct(segment);
	double lengthSeg2();
	int signVectorProduct(point _C);
	int signVectorProduct(segment);
	point rVecPoint();
	point getSegA(); 
	point getSegB(); 
	void setSeg(point, point);
	point p1;
	point p2;
};

segment::segment(point& p1_, point& p2_) {
	p1 = p1_;
	p2 = p2_;
}

point segment::getSegA() {
	return p1;
}

point segment::getSegB() {
	return p2;
}

bool segment::pointOnSeg(point& p) {
	if (p1 < p2) {
		if ((p > p1 || p == p1) && (p < p2 || p == p2)) {
			return true;
		}
	}
	if (p2 < p1) {
		if ((p > p2 || p == p2) && (p < p1 || p == p1)) {
			return true;
		}
	}
	return false;
}

void segment::setSeg(point _A, point _B)
{
	p1 = _A;
	p2 = _B;
}

bool segment::intersectSeg(segment s1) {
	segment s2 = *this;
	if (s1.p1.x > s1.p2.x) {
		double z;
		z = s1.p1.x;
		s1.p1.x = s1.p2.x;
		s1.p2.x = z;
	}
	if (s2.p1.x > s2.p2.x) {
		double z;
		z = s2.p1.x;
		s2.p1.x = s2.p2.x;
		s2.p2.x = z;
	}
	if (s1.p1.y > s1.p2.y) {
		double z;
		z = s1.p1.y;
		s1.p1.y = s1.p2.y;
		s1.p2.y = z;
	}
	if (s2.p1.y > s2.p2.y) {
		double z;
		z = s2.p1.y;
		s2.p1.y = s2.p2.y;
		s2.p2.y = z;
	}
	if (s1.p1.x != s1.p2.x && s2.p1.x != s2.p2.x) {
		double k1, k2;
		k1 = (s1.p2.y - s1.p1.y) / (s1.p2.x - s1.p1.x);
		k2 = (s2.p2.y - s2.p1.y) / (s2.p2.x - s2.p1.x);
		double b1, b2;
		b1 = s1.p1.y - k1*s1.p1.x;
		b2 = s2.p1.y - k2*s2.p1.x;
		if (k1 == k2 && b1 != b2) {
			return false;
		}
		if (k1 == k2 && b1 == b2) {
			if (s1.p2.x < s2.p1.x) {
				return false;
			}
			if (s1.p1.x > s2.p2.x) {
				return false;
			}
			return true;
		}
		point p;
		p.x = (b2 - b1) / (k1 - k2);
		p.y = k1 * p.x + b1;
		if (p.x >= s1.p1.x && p.x <= s1.p2.x && p.y >= s1.p1.y && p.y <= s1.p2.y) {
			return true;
		}
		return false;
	}
	if (s1.p1.x == s1.p2.x && s2.p1.x == s2.p2.x) {
		if (s1.p2.y < s2.p1.y || s1.p1.y > s2.p2.y) {
			return false;
		}
		return true;
	}
	if (s1.p2.x == s1.p1.x) {
		double k2;
		k2 = (s2.p2.y - s2.p1.y) / (s2.p2.x - s2.p1.x);
		double b2;
		b2 = s2.p1.y - k2*s2.p1.x;
		point p;
		p.x = s1.p1.x;
		p.y = k2 * p.x + b2;
		if (p.y >= s1.p1.y && p.y <= s1.p2.y) {
			return true;
		}
		return false;
	}
	double k1;
	k1 = (s1.p2.y - s1.p1.y) / (s1.p2.x - s1.p1.x);
	double b1;
	b1 = s1.p1.y - k1*s2.p1.x;
	point p;
	p.x = s2.p1.x;
	p.y = k1 * p.x + b1;
	if (p.y >= s2.p1.y && p.y <= s2.p2.y) {
		return true;
	}
	return false;
}

bool segment::intersectSeg(segment s1, point& point_) {
	segment s2 = *this;
	if (s1.p1.x > s1.p2.x) {
		double z;
		z = s1.p1.x;
		s1.p1.x = s1.p2.x;
		s1.p2.x = z;
	}
	if (s2.p1.x > s2.p2.x) {
		double z;
		z = s2.p1.x;
		s2.p1.x = s2.p2.x;
		s2.p2.x = z;
	}
	if (s1.p1.y > s1.p2.y) {
		double z;
		z = s1.p1.y;
		s1.p1.y = s1.p2.y;
		s1.p2.y = z;
	}
	if (s2.p1.y > s2.p2.y) {
		double z;
		z = s2.p1.y;
		s2.p1.y = s2.p2.y;
		s2.p2.y = z;
	}
	if (s1.p1.x != s1.p2.x && s2.p1.x != s2.p2.x) {
		double k1, k2;
		k1 = (s1.p2.y - s1.p1.y) / (s1.p2.x - s1.p1.x);
		k2 = (s2.p2.y - s2.p1.y) / (s2.p2.x - s2.p1.x);
		double b1, b2;
		b1 = s1.p1.y - k1*s1.p1.x;
		b2 = s2.p1.y - k2*s2.p1.x;
		if (k1 == k2 && b1 != b2) {
			return false;
		}
		if (k1 == k2 && b1 == b2) {
			if (s1.p2.x < s2.p1.x) {
				return false;
			}
			if (s1.p1.x > s2.p2.x) {
				return false;
			}
			if (s1.p2.x >= s2.p1.x) {
				point_.x = s1.p2.x;
				point_.y = s1.p2.y;
				return true;
			}
			if (s1.p1.x <= s2.p2.x) {
				point_.x = s1.p1.x;
				point_.y = s1.p1.y;
				return true;
			}
			return false;
		}
		point p;
		p.x = (b2 - b1) / (k1 - k2);
		p.y = k1 * p.x + b1;
		if (p.x >= s1.p1.x && p.x <= s1.p2.x && p.y >= s1.p1.y && p.y <= s1.p2.y) {
			point_.x = p.x;
			point_.y = p.y;
			return true;
		}
		return false;
	}
	if (s1.p1.x == s1.p2.x && s2.p1.x == s2.p2.x) {
		if (s1.p2.y < s2.p1.y || s1.p1.y > s2.p2.y) {
			return false;
		}
		if (s1.p2.y >= s2.p1.y) {
			point_.x = s1.p2.x;
			point_.y = s1.p2.y;
			return true;
		}
		if (s1.p1.y <= s2.p2.y) {
			point_.x = s1.p1.x;
			point_.y = s1.p1.y;
			return true;
		}
		return true;
	}
	if (s1.p2.x == s1.p1.x) {
		double k2;
		k2 = (s2.p2.y - s2.p1.y) / (s2.p2.x - s2.p1.x);
		double b2;
		b2 = s2.p1.y - k2*s2.p1.x;
		point p;
		p.x = s1.p1.x;
		p.y = k2 * p.x + b2;
		if (p.y >= s1.p1.y && p.y <= s1.p2.y) {
			point_.x = p.x;
			point_.y = p.y;
			return true;
		}
		return false;
	}
	double k1;
	k1 = (s1.p2.y - s1.p1.y) / (s1.p2.x - s1.p1.x);
	double b1;
	b1 = s1.p1.y - k1*s2.p1.x;
	point p;
	p.x = s2.p1.x;
	p.y = k1 * p.x + b1;
	if (p.y >= s2.p1.y && p.y <= s2.p2.y) {
		point_.x = p.x;
		point_.y = p.y;
		return true;
	}
	return false;
}

point segment::rVecPoint()
{
	double _x = getSegB().x - getSegA().x;
	double _y = getSegB().y - getSegA().y;
	point _C(_x, _y);
	return _C;
}

double segment::vectorProduct(point _C) 
{
	segment _AC(getSegA(), _C);
	point _A(rVecPoint());
	point _B(_AC.rVecPoint());
	return (_A.x * _B.y - _B.x * _A.y);
}

int segment::signVectorProduct(point _C) {
	int k = vectorProduct(_C);
	if (k > 0) {
		return 1;
	}
	if (k < 0) {
		return -1;
	}
	return 0;
}

int segment::signVectorProduct(segment _CD) 
{
	int k = vectorProduct(_CD);
	if (k > 0)
	{
		return 1;
	}
	if (k < 0)
	{
		return -1;
	}
	return 0;
}

double segment::vectorProduct(segment _CD) 
{
	point _A(rVecPoint());
	point _B(_CD.rVecPoint());
	return (_A.x *_B.y - _B.x * _A.y);
}

double segment::lengthSeg2()
{
	return ((getSegA().x - getSegB().x) * (getSegA().x - getSegB().x) +
		(getSegA().y - getSegB().y) * (getSegA().y - getSegB().y));
}

#endif