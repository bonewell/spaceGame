#include "line.hpp"

#include <cmath>

namespace primitive {

double Line::length() const
{
  return std::sqrt(std::pow(begin.x - end.x, 2) +
                   std::pow(begin.y - end.y, 2));
}

Point Line::intersect(Line const& l) const
{
  double x1 = begin.x;
  double y1 = begin.y;

  double x2 = end.x;
  double y2 = end.y;

  double x3 = l.begin.x;
  double y3 = l.begin.y;

  double x4 = l.end.x;
  double y4 = l.end.y;

  double px = ((x1*y2 - y1*x2)*(x3 - x4) - (x1 - x2)*(x3*y4 - y3*x4))/((x1 - x2)*(y3 - y4) - (y1 - y2)*(x3 - x4));
  double py = ((x1*y2 - y1*x2)*(y3 - y4) - (y1 - y2)*(x3*y4 - y3*x4))/((x1 - x2)*(y3 - y4) - (y1 - y2)*(x3 - x4));

  return {px, py};
}
}  // namespace primitive
