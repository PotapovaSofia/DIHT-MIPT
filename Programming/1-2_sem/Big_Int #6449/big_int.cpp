#include "big_int.h"

inline int max(int a, int b) 
{
	return ( ((a) > (b)) ? (a) : (b) );
}

BigInt::BigInt()
	: _sign(true), _data()
{}

BigInt::BigInt(const int a)
	: _sign(true), _data(a, 0)
{}

BigInt::BigInt(const BigInt& a)
	: _sign(a._sign), _data(a._data)
{}

bool BigInt::operator == (const BigInt& a) const
{
	if ( (_sign == a._sign) && (_data == a._data) )
		return true;
	return false;
}

bool BigInt::operator > (const BigInt& a) const
{
	if (_sign > a._sign)
		return true;
	if (a._sign > _sign)
		return false;
	int i = 0;
	for(;;)													// vector iterator (от начала)
	{
		if (_data[i] > a._data[i])
			return true;
		if (a._data[i] > _data[i])
			return false;
	}
	return false;
}

void BigInt::operator = (const BigInt& a)
{
	_sign = a._sign;
	_data = a._data;
}

BigInt BigInt::operator + (const BigInt& b) const
{
	BigInt a(b);
	int i, carry = 0;
	if (_sign == b._sign)
	{	
		for (size_t i = 0; i < max(_data.size(),b._data.size()) || carry; ++i) {
			if (i == a._data.size())
				a._data.push_back(0);
			a._data[i] = carry + (i < b._data.size() ? b._data[i] : 0) + (i < _data.size() ? _data[i] : 0);
			carry = a._data[i] >= BASE;
			if (carry)  a._data[i] -= BASE;
		}
		if (_sign)
			a._sign = true;
		else a._sign = false;
	} else								// different signs
		{
			if (_sign)
				a = *this - b;
			else a = b - *this;
		}
	return a;
}

BigInt BigInt::operator - (const BigInt& b) const
{
	BigInt res(b);
	if (_sign == b._sign)  
		if (_sign)			// x - y
		{
			if (*this > b)
			{
				int i = 0;
				bool carry = 0;
				for(i = max(_data.size(), b._data.size()); i >= 0; --i)
				{
					if(_data[i] < b._data[i])
					{
						res._data[i] = BASE + _data[i] - b._data[i] - carry;
						carry = 1;
					}
					else
						res._data[i] = _data[i] - b._data[i] - carry;
				}
				res._sign = true;
				return res;
			}
			else			// smaller(this) - bigger(b) = -(bigger - smaller) 
			{
				BigInt b2(b), this2(*this);
				res = b2 - this2;
				res._sign = false;
				return res;
			}
		}
		else				// (-x) - (-y) = y - x
		{
			BigInt b2(b), this2(*this);
			b2._sign = true;
			this2._sign = true;
			res = b2 - this2;
			return res;
		}
	else
	{
		if(_sign)			// x - (-y) = x + y
		{
			BigInt b2(b);
			b2._sign = true;
			res = *this + b2;
		} else				// (-x) - y = -(x + y)
			{
				BigInt b2(b), this2(*this);
				b2._sign = true;
				this2._sign = true;
				res = b2 + this2;
				res._sign = false;
			}
	}
	return res;
}

BigInt BigInt::operator * (const BigInt& a) const
{
	int carry;
	BigInt result(a._data.size() + _data.size());
	result._sign = _sign * a._sign;
	for (size_t i = 0; i < _data.size(); ++i)
		for (int j = 0, carry = 0; j < (int)a._data.size() || carry; ++j) 
		{
			long long temp = result._data[i+j] + _data[i] * 1ll * (j < (int)a._data.size() ? a._data[j] : 0) + carry;
			result._data[i+j] = int (temp % BASE);
			carry = int (temp / BASE);
		}
	while (result._data.size() > 1 && result._data.back() == 0)
		result._data.pop_back();
	return result;
}

BigInt BigInt::operator * (const int& a) const
{
	int carry;
	BigInt result( _data.size() );
	result._sign = _sign * (a > 0 ? 1 : -1);
	for (size_t i = 0; i < _data.size() || carry; ++i) 
	{
		if (i == result._data.size())
			result._data.push_back(0);
		long long temp = carry + _data[i] * 1ll * a;
		result._data[i] = int (temp % BASE);
		carry = int (temp / BASE);
	}
	while (result._data.size() > 1 && result._data.back() == 0)
		result._data.pop_back();
	return result;
}

BigInt BigInt::operator / (const int& a) const
{
	int carry = 0;
	BigInt result( _data.size() );
	result._sign = _sign * (a > 0 ? 1 : -1);
	for (int i = _data.size() - 1; i >= 0; --i) 
	{
		long long temp = _data[i] + carry * 1ll * BASE;
		result._data[i] = int (temp / a);
		carry = int (temp % a);
	}	
	while (result._data.size() > 1 && result._data.back() == 0)
		result._data.pop_back();
	return result;
}

BigInt BigInt::operator / (const BigInt& a) const
{
	int carry = 0;
	BigInt result( max(_data.size(), a._data.size()) );
	result._sign = _sign * a._sign;


	/*for (size_t i = 0; i < _data.size(); ++i)
		for (int j = 0, carry = 0; j < (int)a._data.size() || carry; ++j) 
		{
			long long temp = result._data[i+j] + _data[i] * 1ll * (j < (int)a._data.size() ? a._data[j] : 0) - carry;
			result._data[i+j] = int (temp % BASE);
			carry = int (temp / BASE);
		}
	*/

	while (result._data.size() > 1 && result._data.back() == 0)
		result._data.pop_back();
	return result;
}