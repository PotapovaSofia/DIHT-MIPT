#include "kvater.h"
#include <iostream>

using namespace std;

kvaternion::kvaternion(double const x)
	: _a(x), _b(x), _c(x), _d(x)
{}

kvaternion::kvaternion(double a, double b, double c, double d)
	: _a(a), _b(b), _c(c), _d(d)
{} 

kvaternion::~kvaternion()
{}

kvaternion kvaternion::operator + (const kvaternion &other) const
{
	kvaternion c(_a + other._a, _b + other._b, _c + other._c, _d + other._d);
	return c;
}

kvaternion kvaternion::operator - (const kvaternion &other) const
{
	kvaternion c(_a - other._a, _b - other._b, _c - other._c, _d - other._d);
	return c;
}

kvaternion kvaternion::operator * (const kvaternion &other) const
{

}

kvaternion kvaternion::operator / (const kvaternion &other) const
{

}

void kvaternion::operator = (const kvaternion &other)
{	
	_a = other._a;
	_b = other._b;
	_c = other._c;
	_d = other._d;
}

void kvaternion::operator += (const kvaternion &other)
{
	_a += other._a;
	_b += other._b;
	_c += other._c;
	_d += other._d;
}

void kvaternion::operator *= (const kvaternion &other)
{
	*this = *this * other;
}

void kvaternion::operator /= (const kvaternion &other)
{
	*this = *this / other;
}

bool  kvaternion::operator == (const kvaternion &other) const
{
	if ( (_a == other._a) && (_b == other._b) && (_c == other._c) && (_c == other._c) )
		return true;
	return false;
}

double kvaternion::module (const kvaternion &kv) const
{
	return sqrt( kv._a * kv._a +
				 kv._b * kv._b + 
				 kv._c * kv._c + 
				 kv._d * kv._d);
}

kvaternion kvaternion::conjugate (const kvaternion &kv) const
{
	return kvaternion(kv._a, -kv._b, -kv._c, -kv._d);
}

double kvaternion::dot (const kvaternion &kv1, const kvaternion &kv2) const
{
	return( kv1._a * kv2._a +
			kv1._b * kv2._b +
			kv1._c * kv2._c +
			kv1._d * kv2._d );
}

kvaternion kvaternion::cross (const kvaternion &kv1, const kvaternion &kv2) const
{
	return kvaternion(	kv1._a * kv2._a,
						kv1._c*kv2._d - kv1._d*kv2._c,
						kv1._d*kv2._b - kv1._b*kv2._d,
						kv1._b*kv2._c - kv1._c*kv2._b );
}

void print(const kvaternion &kv)
{
	printf("Kvaternion = %lf + (%lf)i (%lf)j + (%lf)k", kv._a, kv._b, kv._c, kv._d);
}

