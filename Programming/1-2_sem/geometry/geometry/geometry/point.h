#include <iostream>

#ifndef POINT_INCLUDED
#define POINT_INCLUDED

class point {
public:
	point(){ }
	point(double, double);
	double x;
	double y;
	point& operator+(point&);
	point& operator-(point&);
	bool operator==(point&);
	bool operator<(point&);
	bool operator>(point&);
};

point::point(double x_, double y_) {
	x = x_;
	y = y_;
}

point& point::operator+(point& a) {
	x += a.x;
	y += a.y;
	return *this;
}

point& point::operator-(point& a) {
	x -= a.x;
	y -= a.y;
	return *this;
}

bool point::operator==(point& p) {
	if (x == p.x && y == p.y) {
		return true;
	}
	return false;
}

bool point::operator>(point& p) {
	if (x > p.x || (x == p.x && y > p.y)) {
		return true;
	}
	return false;
}

bool point::operator<(point& p) {
	if (*this > p || *this == p) {
		return false;
	}
	return true;
}
#endif