#ifndef CIRCLE_H_INCLUDED
#define CIRCLE_H_INNCLUDED

#include "shape.h"
#include "segment.h"
#include <cmath>
#include <math.h>
#define _USE_MATH_DEFINES

class circle : public shape {
public:
	circle(point _O = point(), double _r = 0.0);
	circle(circle& _C);

	void setPointO(point _O = point());
	void setRadius(double _r = 0.0);

	circle getCircle();
	point getPointO();
	double getRadius();

	double area();
	bool pointOnBorder(point& _A);
	bool pointOnShape(point& _A);
	bool interSegShape(segment& _AB, point&);

protected:
	point O;
	double r;
};

circle::circle(point _O, double _r) : O(_O), r(_r) { }

circle::circle(circle& _C) : O(_C.getPointO()), r(_C.getRadius()) { }

void circle::setPointO(point _O) {
	O = _O;
}

void circle::setRadius(double _r) {
	r = _r;
}

circle circle::getCircle() {
	return *this;
}

point circle::getPointO() {
	return O;
}

double circle::getRadius() {
	return r;
}

double circle::area() {
	return 3 * getRadius() * getRadius();
}

bool circle::pointOnBorder(point& _A) {
	if ((_A.x  - getPointO().x) * (_A.x - getPointO().x) +(_A.y - getPointO().y)*(_A.y - getPointO().y) == getRadius() * getRadius()) {
		return true;
	}
	return false;
}

bool circle::pointOnShape(point& _A) {
	if ((_A.x - getPointO().x) * (_A.x - getPointO().x) + (_A.y - getPointO().y)*(_A.y - getPointO().y) <= getRadius() * getRadius()) {
		return true;
	}
	return false;
}

bool circle::interSegShape(segment& _AB, point& _C) {
	if (_AB.p1.x == _AB.p2.x && _AB.p1.y == _AB.p2.y){
		point p;
		p.x = _AB.p1.x;
		p.y = _AB.p1.y;
		return pointOnShape(p);
	}
	double r, a, b, c; 
	r = getRadius();
	if (_AB.p1.x == _AB.p2.x) {
		a = 1;
		b = 0;
		c = -_AB.p1.x;
	}
	else {
		a = (_AB.p1.y - _AB.p2.y) / (_AB.p1.x - _AB.p2.x);
		c = _AB.p1.x - a * _AB.p1.x;
		b = -1; 
	}
	if (getPointO().x != 0 || getPointO().y != 0) {
		c = c - getPointO().x * a - getPointO().y * b;
	}
	const double EPS = 1E-9;
	double x0 = - a * c / (a * a + b * b), y0 = -b * c / (a * a + b * b);
	if (c * c > r * r * (a * a + b * b) + EPS)
		return false;
	else if (abs(c * c - r * r * (a * a + b * b)) < EPS) {
		_C.x = x0;
		_C.y = y0;
		std::cout << x0 << ' ' << y0 << '\n';
		return true;
	}
	else {
		double d = r * r - c * c / (a * a + b * b);
		double mult = sqrt(d / (a * a + b * b));
		double ax, ay, bx, by;
		ax = x0 + b * mult;
		bx = x0 - b * mult;
		ay = y0 - a * mult;
		by = y0 + a * mult;
		std::cout << ax << ' ' << ay << '\n' << bx << ' ' << by << '\n';
		_C.x = ax;
		_C.y = ay;
		return true;
	}
}

#endif // CIRCLE_H_INCLUDED