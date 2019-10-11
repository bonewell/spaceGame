#ifndef PRIMITIVE_LINE_HPP_
#define PRIMITIVE_LINE_HPP_

#include "point.hpp"

namespace primitive {

struct Line {
    Point begin;
    Point end;
    double length() const;
    Point intersect(Line const& l) const;
};

}  // namespace primitive

#endif /* PRIMITIVE_LINE_HPP_ */
