#ifndef CONVPOLYGON_H_INCLUDED
#define CONVPOLYGON_H_INCLUDED

#include "polygon.h"
#include <algorithm>
#include <vector>

bool cmp(point a, point b) {
	return a.x < b.x || a.x == b.x && a.y < b.y;
}

class convpolygon : public polygon {
public:

	convpolygon(int _psize = 0);
	convpolygon(std::vector <point> _ppoints);
	convpolygon(const convpolygon& other);

	bool pointOnBorder(point& _A);
	bool pointOnShape(point& _A);
	bool interSegShape(segment& _AB, point& _C);

	double area();

	point& operator [] (int index);
private:
	bool cw(point, point, point);
	bool ccw(point, point, point);
};
convpolygon::convpolygon(int _psize): polygon(_psize) {
	if (psize() < 3) {
		std::cout << "Error" << std::endl;
		exit(1);
	}
}

convpolygon::convpolygon(std::vector <point> _ppoints): polygon(_ppoints) {
	if (psize() < 3) {
		std::cout << "Error" << std::endl;
		exit(1);
	}
	if (_ppoints.size() == 1) {
		return;
	}
	std::sort(_ppoints.begin(), _ppoints.end(), &cmp);
	point p1 = _ppoints[0], p2 = _ppoints.back();
	std::vector <point> up, down;
	up.push_back(p1);
	down.push_back(p1);
	for (size_t i = 1; i< _ppoints.size(); ++i) {
		if (i == _ppoints.size() - 1 || cw(p1, _ppoints[i], p2)) {
			while (up.size() >= 2 && !cw(up[up.size() - 2], up[up.size() - 1], _ppoints[i]))
				up.pop_back();
			up.push_back(_ppoints[i]);
		}
		if (i == _ppoints.size() - 1 || ccw(p1, _ppoints[i], p2)) {
			while (down.size() >= 2 && !ccw(down[down.size() - 2], down[down.size() - 1], _ppoints[i]))
				down.pop_back();
			down.push_back(_ppoints[i]);
		}
	}
	_ppoints.clear();
	for (size_t i = 0; i<up.size(); ++i)
		_ppoints.push_back(up[i]);
	for (size_t i = down.size() - 2; i>0; --i)
		_ppoints.push_back(down[i]);
}

convpolygon::convpolygon(const convpolygon& other) {
	ppoints.resize(other.ppoints.size());
	for (int i = 0; i < other.ppoints.size(); ++i) {
		ppoints[i] = other.ppoints[i];
	}
}
	
double convpolygon::area() {		
	double S = 0.0;
	for (int i = 1; i < psize() - 1; ++i) {
		segment _AB((*(const_cast<convpolygon*>(this)))[0], (*(const_cast<convpolygon*>(this)))[i]);
		S += _AB.vectorProduct((*(const_cast<convpolygon*>(this)))[i + 1]) / 2;
	}
	return ((S > 0.0) ? S : -S);
}

bool convpolygon::pointOnBorder(point& _A) {
	for (int i = 1; i < psize(); ++i) {
		segment _AB((*(const_cast<convpolygon*>(this)))[i], (*(const_cast<convpolygon*>(this)))[i - 1]);
		if (_AB.pointOnSeg(_A)) {
			return true;
		}
	}
	segment _AB((*(const_cast<convpolygon*>(this)))[0], (*(const_cast<convpolygon*>(this)))[psize() - 1]);
	if (_AB.pointOnSeg(_A)) {
		return true;
	}
	return false;
}

bool convpolygon::pointOnShape(point& _A) {
	int var;
	segment _AB((*(const_cast<convpolygon*>(this)))[0], (*(const_cast<convpolygon*>(this)))[psize() - 1]);
	var = _AB.signVectorProduct(_A);
	if (var != 0) {
		for (int i = 1; i < psize(); ++i) {
			_AB.setSeg((*(const_cast<convpolygon*>(this)))[i], (*(const_cast<convpolygon*>(this)))[i - 1]);
			if (-var == _AB.signVectorProduct(_A)) {
				return false;
			}
		}
		return true;
	}
	else {
		for (int i = 1; i < psize() - 1; ++i) {
			_AB.setSeg((*(const_cast<convpolygon*>(this)))[i], (*(const_cast<convpolygon*>(this)))[i - 1]);
			int _var = _AB.signVectorProduct(_A);
			if ((var != 0) && (-var == _var)) {
				return false;
			}
			if ((var == 0) && (var != _var)) {
				var = _var;
			}
		}
		if (var != 0) {
			_AB.setSeg((*(const_cast<convpolygon*>(this)))[psize() - 1], (*(const_cast<convpolygon*>(this)))[psize() - 2]);
			if (-var == _AB.signVectorProduct(_A)) {
				return false;
			}
			return true;
		}
		return true;
	}
}

bool convpolygon::interSegShape(segment& _AB, point& _C) {
	for (int i = 1; i < psize(); ++i) {
		segment _CD((*(const_cast<convpolygon*>(this)))[i], (*(const_cast<convpolygon*>(this)))[i - 1]);
		if (_CD.intersectSeg(_AB, _C)) {
			return true;
		}
	}
	segment _CD((*(const_cast<convpolygon*>(this)))[psize() - 1], (*(const_cast<convpolygon*>(this)))[0]);
	if (_CD.intersectSeg(_AB, _C)) {
    	return true;
	}
	return false;
}

point& convpolygon::operator [] (int index) {
	if ((index < 0) || (index >= psize())) {
		std::cout << "Error index" << std::endl;
		exit(1);
	}
	return ppoints[index];
}

bool convpolygon::cw(point a, point b, point c) {
	return a.x*(b.y - c.y) + b.x*(c.y - a.y) + c.x*(a.y - b.y) < 0;
}

bool convpolygon::ccw(point a, point b, point c) {
	return a.x*(b.y - c.y) + b.x*(c.y - a.y) + c.x*(a.y - b.y) > 0;
}


#endif CONVPOLYGON_H_INCLUDED