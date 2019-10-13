#ifndef SPACE_SHIP_HPP_
#define SPACE_SHIP_HPP_

#include <memory>
#include <vector>

#include "colorschema.hpp"
#include "speed.hpp"

#include "object.hpp"
#include "nozzle.hpp"
#include "primitive/color.hpp"
#include "primitive/point.hpp"
#include "primitive/size.hpp"
#include "scene/scene.hpp"
#include "space/projectile.hpp"


namespace space {

class Ship: public Object {
public:
  using Border = std::vector<primitive::Point>;

  static constexpr double kAngle{M_PI/15};
  static constexpr auto kRotationDelay = 60ms;
  static constexpr auto kShootingDelay = 100ms;
  static constexpr int kMaxSpeed{50};
  static constexpr primitive::Size size{30, 60};
  const ColorSchema colors{{255, 255, 0}, {255,8,0}};
  const ColorGenerator cologen{{220,220,220}, {192,192,192}, {105,105,105},
                                   {211,211,211}, {119,136,153}};

  explicit Ship(primitive::Point coordinate);
  Border const& get_border() const { return border_; }
  Nozzle const& get_left_nozzle() const { return left_nozzle_; }
  Nozzle const& get_right_nozzle() const { return right_nozzle_; }
  Speed const& get_speed() const { return speed_; }
  primitive::Point const& get_initial_median_intersaction() const
  { return initial_median_intersection_; }
  double getTiltAngel() const;
  void slowdown();
  void accelarate();
  void backwardSlowdown();
  void backwardAccelarate();
  void rotate(bool clockwise);
  void rotatePointsInVector(std::vector<primitive::Point> &points,
                            primitive::Point initial_median_intersection,
                            double angle) const;
  ProjectilePtr shoot();
  primitive::Point CalcMedianIntersaction() const;
  primitive::Direction getOffset();

  void update() override;
  void display(scene::Scene& scene) const override { scene.draw(*this); }

private:
  Border border_;
  Nozzle left_nozzle_;
  Nozzle right_nozzle_;
  Speed speed_{kMaxSpeed};
  primitive::Time rotation_delay_;
  primitive::Point initial_median_intersection_;
  primitive::Time shoot_delay_;
};

}  // namespace space

#endif /* SPACE_SHIP_HPP_ */
