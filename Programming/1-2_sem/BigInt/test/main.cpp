#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <sstream>
#include <string.h>
#include <math.h>
#include <cmath>
#include <string>
#include <algorithm>
#include <cstdlib>

#define max(x,y) ((x) > (y) ? (x) : (y))
#define abs(x) ((x) >= 0 ? (x) : (-x))

using namespace std;

class BigInt_mine
{
    public:
    vector<int> digits;
    int sign;


        BigInt_mine();
        BigInt_mine(int n);
        BigInt_mine(string str);
        BigInt_mine(const BigInt_mine &t);
        ~BigInt_mine();
        void myfunc(int i);
        friend istream& operator>> (istream& istr, BigInt_mine &t);
        void _cout();
        BigInt_mine f_minus(const BigInt_mine &a, const BigInt_mine &b, int sign_of_c);
        friend bool operator>(const BigInt_mine &a, const BigInt_mine &b);
        friend bool operator>=(const BigInt_mine &a, const BigInt_mine &b);
        BigInt_mine operator+(const BigInt_mine &t);
        BigInt_mine operator-(const BigInt_mine &t);
        BigInt_mine operator*(const BigInt_mine &t);
        BigInt_mine operator*(int n);
        BigInt_mine operator/(const BigInt_mine &t);
        BigInt_mine operator/(int n);
        BigInt_mine operator%(const BigInt_mine &t);
        BigInt_mine operator%(int n);




};
BigInt_mine::BigInt_mine()
{
    vector<int> digits;
    sign = 1;
}
BigInt_mine::BigInt_mine(int n)
{
    int length = 0;
    int n_ = abs(n);
    if (n!=0)
    {
        if (n > 0)
            sign = 1;
        if (n < 0)
            sign = -1;

        while(n_!=0)
        {
            length++;
            n_=n_/10;
        }
        for (n_=0; n_<length; n_++)
        {
            digits.push_back(abs(n)%10);
            n=n/10;
        }
    }
    else
    {
        sign = 0;
        digits.push_back(0);
    }
}
BigInt_mine::BigInt_mine(const BigInt_mine &t)
{
    sign=t.sign;
    digits = t.digits;
}
BigInt_mine::~BigInt_mine()
{

}
void BigInt_mine::myfunc(int i)
{
	cout<<i;
}
istream& operator>> (istream& istr, BigInt_mine &t)
{
	string c;
	string k;
	istr >> c;
	int i,l;
	switch(c[0])
	{
	case '-':t.sign = -1;c.erase(0,1);break;
	case '0':t.sign = 0;break;
	default:t.sign = 1;break;
	}
	i = c.length()-1;
	while(i>=0)
	{
		k = c.substr(i,1);
		t.digits.push_back(atoi(k.c_str()));
		i--;
	}
	return (istr);
}
void BigInt_mine::_cout()
{
    BigInt_mine t = *this;
    if(t.sign==0) cout << "0";
    else
    {
        if (t.sign==-1) cout << "-";
        for (int i = t.digits.size() - 1; i >= 0; i--)
        {
                cout << t.digits[i];
        }
        cout << "\n";
    }
}
BigInt_mine BigInt_mine::f_minus(const BigInt_mine &a, const BigInt_mine &b, int sign_of_c)
{
    //a>0; b>0; a>b; length_a == length_b
    BigInt_mine _c = BigInt_mine();
    int i, res;
    int modulo = 0;
    int l = a.digits.size();
    for(i = 0; i < l; i++)
            {
                if(a.digits[i]>=b.digits[i]+modulo)
                {
                    res = a.digits[i] - b.digits[i] - modulo;
                    modulo = 0;
                }
                else
                {
                    res = 10 + a.digits[i] - b.digits[i] - modulo;
                    modulo = 1;
                }
                _c.digits.push_back(res);
            }
    _c.sign = sign_of_c;
    return _c;
}
bool operator>(const BigInt_mine &a, const BigInt_mine &b)
{
    int la = a.digits.size();
    int lb = b.digits.size();
    if (a.sign == 0)
    {
       if (b.sign == 1) return false;
       else if (b.sign == -1) return true;
       else return false;

    }
    if (b.sign == 0)
    {
        if (a.sign == 1) return true;
        else if (a.sign == -1) return false;
    }
    if((a.sign == b.sign)&&(a.sign == 1))
    {
        if (la > lb) return true;
        else if (la < lb) return false;
        else
        {
            for(int i = la - 1; i >= 0; i--)
            {
                if(a.digits[i]>b.digits[i]) return true;
                else if(a.digits[i]<b.digits[i]) return false;
            }
        }
        return false;//a=b
    }
    if ((a.sign == b.sign) && (a.sign == -1))
    {
        if (la > lb) return false;
        else if (la < lb) return true;
        else
        {
            for(int i = la - 1; i >= 0; i--)
            {
                if(a.digits[i]>b.digits[i]) return false;
                else if(a.digits[i]<b.digits[i]) return true;
            }
        }
        return false;//a=b
    }
    if ((a.sign == -1) && (b.sign == 1))
        return false;
    if ((a.sign == 1) && (b.sign == -1))
        return true;


}
bool operator>=(const BigInt_mine &a, const BigInt_mine &b)
{
    int la = a.digits.size();
    int lb = b.digits.size();
    if (a.sign == 0)
    {
       if (b.sign == 1) return false;
       else if (b.sign == -1) return true;
       else return true;

    }
    if (b.sign == 0)
    {
        if (a.sign == 1) return true;
        else if (a.sign == -1) return false;
    }
    if((a.sign == b.sign)&&(a.sign == 1))
    {
        if (la > lb) return true;
        else if (la < lb) return false;
        else
        {
            for(int i = la - 1; i >= 0; i--)
            {
                if(a.digits[i]>b.digits[i]) return true;
                else if(a.digits[i]<b.digits[i]) return false;
            }
        }
        return true;//a=b
    }
    if ((a.sign == b.sign) && (a.sign == -1))
    {
        if (la > lb) return false;
        else if (la < lb) return true;
        else
        {
            for(int i = la - 1; i >= 0; i--)
            {
                if(a.digits[i]>b.digits[i]) return false;
                else if(a.digits[i]<b.digits[i]) return true;
            }
        }
        return true;//a=b
    }
    if ((a.sign == -1) && (b.sign == 1))
        return false;
    if ((a.sign == 1) && (b.sign == -1))
        return true;


}
BigInt_mine BigInt_mine::operator+(const BigInt_mine &t)
{
	BigInt_mine _c = BigInt_mine();
	BigInt_mine _a = *this;
	BigInt_mine _b = t;
	int modulo = 0;
	int sign_of_c = 1;
	int k,i,maximum,res,j;
	int len_a,len_b;
	len_a = _a.digits.size();
	len_b = _b.digits.size();
	maximum = max(len_a,len_b);
	if (_a.sign==0)
	{
		return _b;
	}
	if (_b.sign==0)
	{
		return _a;
	}
	if (_a.sign==_b.sign)
	{
		if (_a.sign==-1)
         {
             sign_of_c = -1;
             _a.sign = 1;
             _b.sign = 1;
         }
		for (i=0;i<maximum;i++)
		{
			if (len_a > len_b)
            {
                for(k = len_a - len_b; k > 0; k--)
                    _b.digits.push_back(0);
            }
            if (len_a < len_b)
            {
                for(k = len_b - len_a; k > 0; k--)
                    _a.digits.push_back(0);
            }
			res = _a.digits[i] + _b.digits[i] + modulo;
			modulo = res/10;
			_c.digits.push_back(res%10);
		}

		if (modulo!=0)
		{
			_c.digits.push_back(modulo);
		}
	}
	if ((_a.sign==1)&&(_b.sign==-1))
	{
		_a.sign = 1;
		_b.sign = 1;
		_c = _a - _b;
	}
	if ((_a.sign==-1)&&(_b.sign==1))
	{
		_a.sign = 1;
		_b.sign = 1;
		_c = _a - _b;
	}
    _c.sign = sign_of_c;
return _c;
}
BigInt_mine BigInt_mine::operator-(const BigInt_mine &t)
{
    BigInt_mine _c = BigInt_mine();
    BigInt_mine _a = *this;
    BigInt_mine _b = t;
    int length_a = _a.digits.size();
    int length_b = _b.digits.size();
    int _max = max(length_a, length_b);
    if (_a.sign==0)
    {
        _b.sign *= -1;
        return _b;
    }
    if (_b.sign==0)
        return _a;
    int sign_of_c = 1;
    int i;
    if (_a.sign == _b.sign)
    {
       if(_a.sign == -1)
       {
           _a.sign=1;
           _b.sign=1;
           sign_of_c = -1;
       }
       if (_a > _b)
       {
           if (length_a > length_b)
            {
                for(i = length_a - length_b; i > 0; i--)
                    _b.digits.push_back(0);
            }
            _c = f_minus(_a, _b, sign_of_c);
       }
       else if (_b > _a)
       {
           if (length_a < length_b)
            {
                for(i = length_b - length_a; i > 0; i--)
                    _a.digits.push_back(0);
            }
            _c = f_minus(_b, _a, -1);
       }
       else //_a == _b
       {
            _c.sign = 0;
            _c.digits.clear();
        }
    }
    if((_a.sign == 1)&&(_b.sign == -1))
    {
        _b.sign = 1;
        _c = _a + _b;
    }
    if((_a.sign == -1)&&(_b.sign == 1))
    {
        _a.sign = 1;
        _c = _a + _b;
        _c.sign = -1;
    }
    i = _max - 1;
    while(_c.digits[i] == 0)
        {
            _c.digits.pop_back();
            i--;
        }
   // _c.sign = sign_of_c;
    return _c;
}
BigInt_mine BigInt_mine::operator*(const BigInt_mine &t)
{
    BigInt_mine _c = BigInt_mine();
	BigInt_mine _a = *this;
	BigInt_mine _b = t;
	int l;
	if (_a.sign==0 || _b.sign==0)
	{
		_c.sign = 0;
		_c.digits[0]=0;
	}
	else
	{
		(_a.sign * _b.sign == 1)?(_c.sign=1):(_c.sign=-1);
	}
	for (int i = 0; i < ( l = (_a.digits.size()*_b.digits.size() + 1));++i)
	{
		_c.digits.push_back(0);
	}

	int i, j, modulo, new_c;
    for( i = 0; i < _b.digits.size(); i++ )
    {
        if( _b.digits[i] )
        {
            modulo = 0;
            for( j = 0; j < _a.digits.size() || modulo; j++ )
            {
                new_c = _c.digits[i + j] + ( j < _a.digits.size() ? _b.digits[i] * _a.digits[j] : 0 ) + modulo;
                _c.digits[i + j] = new_c % 10;
                modulo = new_c / 10;
            }
        }
    }

	while((_c.digits[l-1]==0)&&(l!=1))
	{
		_c.digits.pop_back();
		--l;
	}
	return _c;
}
BigInt_mine BigInt_mine::operator*(int n)
{
	int flag = 0;
	BigInt_mine _c = BigInt_mine();
	BigInt_mine _a = *this;
	_c.digits.resize(_a.digits.size()+1);
	if (n == 1)
		return _a;
	if (_a.sign==0 || n==0)
	{
		_c.sign = 0;
		_c.digits[0] = 0;
	}
	else
	{
		((_a.sign * n) > 0)?(_c.sign=1):(_c.sign=-1);
	}
	int i;
	for (i=0; i<_a.digits.size(); ++i)
	{
		_c.digits[i] = flag + _a.digits[i] * n;
		flag = _c.digits[i]/10;
		_c.digits[i] = _c.digits[i]%10;
	}
	_c.digits[i] = flag;
	while (_a.digits.size() >= 1 && _c.digits.back() == 0)
		_c.digits.pop_back();
	return _c;
}
BigInt_mine BigInt_mine:: operator/( const BigInt_mine &t)
{
    BigInt_mine _c = BigInt_mine();
	BigInt_mine _a = *this;
	BigInt_mine _b = t;

	if (_b.sign==0)
	{
		fprintf(stderr,"Error\n");
		exit(1);
	}
	if ((_a.digits.size()<_b.digits.size())||(_a.sign==0))
	{
		_c.sign = 0;
		_c.digits.push_back(0);
	}
	(_a.sign * _b.sign == 1)?(_c.sign = 1):(_c.sign = -1);
	int sign_of_a = _a.sign;
	int sign_of_b = _b.sign;
	_a.sign = 1;
	_b.sign = 1;

	if ((_a>=_b)&&(_b>=_a))
	{
		_c.digits.push_back(1);
		return _c;
	}
	if ((_b.digits[0]==1)&&(_b.digits.size() == 1))
		return _a;
    if (_b > _a)
    {
        _c.digits.push_back(0);
        _c.sign = 0;
        return _c;
    }


 	int res;
	BigInt_mine cur = BigInt_mine();
	int i = _a.digits.size() - 1;
	int flag = 1;
	int flag1 = 0;
	while ( flag )
    {
        while (_b > cur)
        {
            reverse(cur.digits.begin(),cur.digits.end());
            cur.digits.push_back(_a.digits[i]);
            reverse(cur.digits.begin(),cur.digits.end());
            i--;
            _c.digits.push_back(0);
        }
        res = 0;
        while (cur > _b)
        {
            cur = cur - _b;
            res++;
        }
        if ((cur >= _b) && (_b >= cur))
        {
            res++;
            cur.digits.clear();
            flag1 = 1;
        }
        _c.digits.push_back(res);

        if ( i >= 0 )
        {
            reverse(cur.digits.begin(),cur.digits.end());
            cur.digits.push_back(_a.digits[i]);
            reverse(cur.digits.begin(),cur.digits.end());
            i--;
        }
        else
        {
            if (flag1) cur.digits.push_back(0);
            BigInt_mine modulo = cur;
            flag = 0;
        }
    }
    reverse(_c.digits.begin(),_c.digits.end());
    i = _c.digits.size() - 1;
    while(_c.digits[i] == 0)
    {
        _c.digits.pop_back();
        i--;
    }

    return _c;
}
BigInt_mine BigInt_mine:: operator%( const BigInt_mine &t)
{
    BigInt_mine _c = BigInt_mine();
	BigInt_mine _a = *this;
	BigInt_mine _b = t;
	BigInt_mine modulo = BigInt_mine();

	if (_b.sign==0)
	{
		fprintf(stderr,"Error\n");
		exit(1);
	}
	if ((_a.digits.size()<_b.digits.size())||(_a.sign==0))
	{
		_c.sign = 0;
		_c.digits.push_back(0);
	}
	(_a.sign * _b.sign == 1)?(_c.sign = 1):(_c.sign = -1);
	int sign_of_a = _a.sign;
	int sign_of_b = _b.sign;
	_a.sign = 1;
	_b.sign = 1;

	if ((_a>=_b)&&(_b>=_a))
	{
		_c.digits.push_back(1);
		return _c;
	}
	if ((_b.digits[0]==1)&&(_b.digits.size() == 1))
		return _a;
    if (_b > _a)
    {
        _c.digits.push_back(0);
        _c.sign = 0;
        return _c;
    }


 	int res;
	BigInt_mine cur = BigInt_mine();
	int i = _a.digits.size() - 1;
	int flag = 1;
	int flag1 = 0;
	while ( flag )
    {
        while (_b > cur)
        {
            reverse(cur.digits.begin(),cur.digits.end());
            cur.digits.push_back(_a.digits[i]);
            reverse(cur.digits.begin(),cur.digits.end());
            i--;
            _c.digits.push_back(0);
        }
        res = 0;
        while (cur > _b)
        {
            cur = cur - _b;
            res++;
        }
        if ((cur >= _b) && (_b >= cur))
        {
            res++;
            cur.digits.clear();
            flag1 = 1;
        }
        _c.digits.push_back(res);

        if ( i >= 0 )
        {
            reverse(cur.digits.begin(),cur.digits.end());
            cur.digits.push_back(_a.digits[i]);
            reverse(cur.digits.begin(),cur.digits.end());
            i--;
        }
        else
        {
            if (flag1) cur.digits.push_back(0);
            modulo = cur;
            flag = 0;
        }
    }
    reverse(_c.digits.begin(),_c.digits.end());
    i = _c.digits.size() - 1;
    while(_c.digits[i] == 0)
    {
        _c.digits.pop_back();
        i--;
    }

    return modulo;
}
int main()
{
    /**int n;
    cin >> n;
    int m;
    cin >> m;**/
    BigInt_mine _a = BigInt_mine();
    cin >> _a;
    BigInt_mine _b = BigInt_mine();
    cin >> _b;
    BigInt_mine _c = _a + _b;
    _c._cout();

	return 0;
}
