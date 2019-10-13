#ifndef SPEED_H
#define SPEED_H

#include "primitive/point.hpp"

class Speed {
private:
    double max_offset_{};
    double forward_offset_{};
    double backward_offset_{};
    double negative_offset_{};
    double positive_offset_{};
    double current_a_{};  // TODO: What is it?

public:
    explicit Speed(double);
    void slowdown();
    void accelarate();
    void backwardSlowdown();
    void backwardAccelarate();
    primitive::Direction getOffsetXY(primitive::Direction,
                                     primitive::Direction);
    double get_current_a() const;
};

#endif // SPEED_H
