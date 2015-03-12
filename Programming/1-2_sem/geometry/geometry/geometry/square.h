#ifndef SQUARE_H_INCLUDED
#define SQUARE_H_INCLUDED

#include "rectangle.h"

class square : public rectangle {
public:
	square(int _psize = 0);
	square(std::vector <point> _ppoints);
	square(const square& other);
};

#include "square.h"

square::square(int _psize): rectangle(_psize) {
	segment _AB(ppoints[0], ppoints[1]);
	segment _BC(ppoints[1], ppoints[2]);
	if (_AB.lengthSeg2() != _BC.lengthSeg2()) {
		std::cout << "Error" << std::endl;
		exit(6);
	}
}

square::square(std::vector <point> _ppoints) : rectangle(_ppoints) {
	segment _AB(ppoints[0], ppoints[1]);
	segment _BC(ppoints[1], ppoints[2]);
	if (_AB.lengthSeg2() != _BC.lengthSeg2()) {
		std::cout << "Error" << std::endl;
		exit(6);
	}
}

square::square(const square& other) {
	ppoints.resize(other.ppoints.size());
	for (int i = 0; i < other.ppoints.size(); ++i) {
		ppoints[i] = other.ppoints[i];
	}
}


#endif // SQUARE_H_INCLUDED