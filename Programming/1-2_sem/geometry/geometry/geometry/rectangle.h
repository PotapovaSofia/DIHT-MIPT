#ifndef RECTANGLE_H_INCLUDED
#define RECTANGLE_H_INCLUDED

#include "guadrangle.h"

class rectangle : public quadrangle {
public:
	rectangle(int _psize = 0);
	rectangle(std::vector <point> _ppoints);
	rectangle(rectangle& other);
};

rectangle::rectangle(int _psize ) : quadrangle(_psize) {
	segment _AB(ppoints[0], ppoints[1]);
	segment _CD(ppoints[2], ppoints[3]);
	if (_AB.lengthSeg2() != _AB.lengthSeg2()) {
		std::cout << "Error" << std::endl;
		exit(5);
	}
	if (_AB.signVectorProduct(_CD) != 0) {
		std::cout << "Error" << std::endl;
		exit(5);
	}
	_AB.setSeg(ppoints[0], ppoints[3]);
	_CD.setSeg(ppoints[2], ppoints[1]);
	if (_AB.signVectorProduct(_CD) != 0) {
		std::cout << "Error" << std::endl;
		exit(5);
	}
}

rectangle::rectangle(std::vector <point> _ppoints) : quadrangle(_ppoints) {
	segment _AB(ppoints[0], ppoints[1]);
	segment _CD(ppoints[2], ppoints[3]);
	if (_AB.lengthSeg2() != _AB.lengthSeg2()) {
		std::cout << "Error" << std::endl;
		exit(5);
	}
	if (_AB.signVectorProduct(_CD) != 0) {
		std::cout << "Error" << std::endl;
		exit(5);
	}
	_AB.setSeg(ppoints[0], ppoints[3]);
	_CD.setSeg(ppoints[2], ppoints[1]);
	if (_AB.signVectorProduct(_CD) != 0) {
		std::cout << "Error" << std::endl;
		exit(5);
	}
}

rectangle::rectangle(rectangle& other) {
	ppoints.resize(other.ppoints.size());
	for (int i = 0; i < other.ppoints.size(); ++i) {
		ppoints[i] = other.ppoints[i];
	}
}

#endif // RECTANGLE_H_INCLUDED