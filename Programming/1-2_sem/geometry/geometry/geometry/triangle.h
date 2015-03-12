#ifndef TRIANGLE_H_INCLUDED
#define TRIANGLE_H_INCLUDED

#include "convpolygon.h"

class triangle : public convpolygon {
public:
	triangle(int _psize = 0);
	triangle(std::vector <point> _ppoints);
	triangle(const triangle& other);
};

#include "triangle.h"

triangle::triangle(int _psize) : convpolygon(_psize) {
	if (psize() != 3) {
		std::cout << "Error" << std::endl;
		exit(3);
	}
}

triangle::triangle(std::vector <point> _ppoints) : convpolygon(_ppoints) {
	if (psize() != 3) {
		std::cout << "Error" << std::endl;
		exit(3);
	}
}

triangle::triangle(const triangle& other) {
	ppoints.resize(other.ppoints.size());
	for (int i = 0; i < other.ppoints.size(); ++i) {
		ppoints[i] = other.ppoints[i];
	}
	if (psize() != 3) {
		std::cout << "Error" << std::endl;
		exit(3);
	}
}


#endif // TRIANGLE_H_INCLUDED
