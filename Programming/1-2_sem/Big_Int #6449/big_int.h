#pragma once

#include <vector>
#include <iostream>

class BigInt
{
public:
  BigInt();									// done
  BigInt(const int);						// done
  BigInt(const BigInt&);					// done

  void operator = (const BigInt&);			// done

  BigInt operator + (const BigInt&) const;  // done
  BigInt operator - (const BigInt&) const;	// done
  BigInt operator * (const BigInt&) const;  // done
  BigInt operator / (const BigInt&) const;
  BigInt operator % (const BigInt&) const;
  bool operator > (const BigInt&) const;	// done
  bool operator == (const BigInt&) const;	// done

  BigInt operator * (const int&) const;		// done
  BigInt operator / (const int&) const;
  BigInt operator % (const int&) const;

  std::ostream& operator << (std::ostream& s);
  std::istream& operator >> (std::istream& s);

private:
  /**
  \  arithmetic operations without sign
  */
  
  /**
  \  retval   this + other
  \  note     first may have the same sign as second
  */
  static BigInt add(BigInt const& first, 
                    BigInt const& second);

  /**
  \  retval   this - other
  \  note     should be abs(first) > abs(second)
  */  
  static BigInt dif(BigInt const& first, 
                    BigInt const& second);

  /**
  \  brief remove leading zeros after 
  \  arithmetic operation
  */
  static void rmLeadingZeros(BigInt&);

  /**
  /  brief   bitwise comparison (without sign)
  /  retval  true    if abs(this) > abs(other)
  /          false   else
  */
  bool absCmpr(const BigInt& other) const;

private:
  /**
  \  true    if BigInt >= 0,
  \  false   else
  */
  bool _sign;

  std::vector<int> _data;

  static const int BASE = 10000;
};