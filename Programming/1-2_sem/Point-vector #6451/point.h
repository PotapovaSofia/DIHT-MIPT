#pragma once

class Point
{
public:
  Point();
  Point(int const x, int const y);
  Point(Point const&);

  Point operator + (Point const&) const;
  Point operator - (Point const&) const;
  Point operator = (Point const&) const;
  bool operator == (Point const&) const;

  friend class Segment;
  /*bool intersect(Segment const& s1, 
                        Segment const& s2,
                        Point& ptOfIntersect); */
private:
  double _x;
  double _y;
};