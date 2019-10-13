#include "asteroid.hpp"

#include <algorithm>
#include <cstdlib>

namespace space {

Asteroid::Asteroid(primitive::Point coordinate)
    : coordinate_{coordinate}
{
    display_delay_ = primitive::delay(kDisplayDelay);

    int error_x = rand() % 10;
    int error_y = rand() % 10;
    int size = rand() % 20;

    pp_.push_back({coordinate_.x + error_x, coordinate_.y + error_y});
    pp_.push_back({coordinate_.x - size + rand() % 5, coordinate_.y + size + rand() % 5});
    pp_.push_back({coordinate_.x + rand() % 5, coordinate_.y + size * 2 + rand() % 5});
    pp_.push_back({coordinate_.x + size + rand() % 5, coordinate_.y + size * 2 + rand() % 5});
    pp_.push_back({coordinate_.x + size * 2 + rand() % 5, coordinate_.y + size + rand() % 5});
    pp_.push_back({coordinate_.x + size + rand() % 5, coordinate_.y + rand() % 5});
}

primitive::Point Asteroid::center() const
{
    double x = 0.0, y = 0.0;
    int size = pp_.size();
    for (auto iter = pp_.begin(); iter != pp_.end(); ++iter) {
        x += iter->x / size;
        y += iter->y / size;
    }
    return {x, y};
}

void Asteroid::move(primitive::Direction d)
{
    std::for_each(std::begin(pp_), std::end(pp_), [&d](auto& p) {
        p.move(d);
    });
}

}  // namespace space
