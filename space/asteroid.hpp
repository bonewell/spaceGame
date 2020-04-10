#ifndef SPACE_ASTEROID_H
#define SPACE_ASTEROID_H

#include <memory>
#include <vector>

#include "colorschema.hpp"
#include "object.hpp"
#include "primitive/point.hpp"
#include "primitive/time.hpp"
#include "scene/scene.hpp"

namespace space {

class Asteroid : public Object {
public:
    static constexpr auto kBorder = false;
    static constexpr auto kDisplayDelay = 10ms;
    const ColorGenerator colors{{160, 177, 188}, {119, 98, 84}, {128, 0, 0},
                                {81, 81, 81}, {76, 62, 54}, {139, 69, 19}};

    explicit Asteroid(primitive::Point coordinate);
    void move(primitive::Direction d);
    primitive::Point center() const;
    bool isAlive() { return alive_; }
    void markAsDead() { alive_ = false; }
    const std::vector<primitive::Point>& get_points() const { return pp_; }

    void update() override {}
    void display(scene::Scene& scene) const override { scene.draw(*this); }

private:
    std::vector<primitive::Point> pp_;
    bool alive_{true};
    primitive::Point coordinate_;
    primitive::Time display_delay_;
    primitive::Time rotation_delay_;
};

using AsteroidPtr = std::unique_ptr<Asteroid>;

}  // namespace space

#endif
