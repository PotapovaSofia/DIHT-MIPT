#pragma once

#include "point.h"
#include "segment.h"

class Vector
{
public:
  Vector();
  Vector(Vector  const&);
  Vector(Segment const&);

  static Vector inverse(Vector const&);

private:
  Point _pt0;
  Point _coords;
};