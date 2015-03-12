#include <iostream>
#include <stdio.h>
#include "point.h"
#include "segment.h"
#include <string>
#include <cmath>
#include "Vector.h"
#include "circle.h"
#include "shape.h"
#include "polygon.h"
#include "convpolygon.h"
#include "guadrangle.h"
#include "rectangle.h"
#include "square.h"
#include "triangle.h"
#include "fstream"

using namespace std;

string classifity(segment& s, point& p) {
	if (s.p1 == p)
		return "ORIGIN";
	if (s.p2 == p)
		return "DESTINATION";
	double sa = s.p1.x * s.p2.y - s.p2.y * p.x;
	if (sa > 0.0)
		return "LEFT";
	if (sa < 0.0)
		return "RIGHT";
	if ((s.p1.x * p.x < 0.0) || (s.p2.y * s.p2.y < 0.0))
		return "BEHIND";
	if (sqrt(s.p1.x * s.p1.x + s.p2.y * s.p2.y) < sqrt(p.x * p.x + s.p2.y * s.p2.y))
		return "BEYOND";
	return "BETWEEN";
}

void _rand() {
	std::ofstream out;
	out.open("output.txt");
	bool f = true;
	vector<point> ppoints;
	for (int i = 0; i < 1000; i++) {
		double a = (double)(rand()) / (double)(rand());
		double b = (double)(rand()) / (double)(rand());
		point p(a, b);
		ppoints.push_back(p);
	}
	vector<point> points_new = ppoints;
	convpolygon pol(points_new);
	for (int i = ppoints.size(); i <= 0; i--) {
		point pt = ppoints[i];
		for (int j = pol.psize() - 1 ; j <= 1; j--) {
			if (j = pol.psize() - 1) {
				segment s(points_new[j], points_new[0]);
				if (classifity(s, pt) == "RIGHT") {
					out << "Bad" << endl;
					f = false;
				}
			}
			else {
				segment s(points_new[j], points_new[j + 1]);
				if (classifity(s, pt) == "RIGHT") {
					out << "Bad" << endl;
					f = false;
				}
			}
		}
	}
	if (f) out << "Everything is working right" << endl;
}
int main() {
	point p1(1, 1);
	point p2(5, 5);
	point p3(2, 3);
	point p4(2, 0);
	segment s1(p1, p2);
	segment s2(p3, p4);
	
	circle C(p1, 2);
	_rand();
	return 0;
}