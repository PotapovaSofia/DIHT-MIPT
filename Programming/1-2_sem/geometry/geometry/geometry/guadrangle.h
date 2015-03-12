#ifndef QUADRANGLE_H_INCLUDED
#define QUADRANGLE_H_INCLUDED

#include "convpolygon.h"

class quadrangle : public convpolygon {
public:
	quadrangle(int _psize = 0);
	quadrangle(std::vector <point> _ppoints);
	quadrangle(quadrangle& other);
};

quadrangle::quadrangle(int _psize ) : convpolygon(_psize) {
	if (psize() != 4) {
		std::cout << "Error" << std::endl;
		exit(4);
	}
}

quadrangle::quadrangle(std::vector <point> _ppoints): convpolygon(_ppoints) {
	if (psize() != 4) {
		std::cout << "Error" << std::endl;
		exit(4);
	}
}

quadrangle::quadrangle(quadrangle& other) {
	ppoints.resize(other.ppoints.size());
	for (int i = 0; i < other.ppoints.size(); ++i) {
		ppoints[i] = other.ppoints[i];
	}
	if (psize() != 4) {
		std::cout << "Error" << std::endl;
		exit(4);
	}
}


#endif // QUADRANGLE_H_INCLUDED