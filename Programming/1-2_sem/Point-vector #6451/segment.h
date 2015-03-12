#pragma once

#include "point.h"

class Segment
{
public:
  Segment();

  Segment(int const x0,
          int const y0, 
          int const x1, 
          int const y1);

  Segment(Point const& p0, 
          Point const& p1);
  
  static bool intersect(Segment const& s1, 
                        Segment const& s2,
                        Point& ptOfIntersect);

  static bool isPointOnSegment(const Point&, 
                             const Segment&);

  static int rotateSegmentAroundPoint(const Point&, 
                                    const Segment&);

  friend class Vector;
private:
  Point _p0;
  Point _p1;
};