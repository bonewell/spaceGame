#include "skeleton.hpp"

#include <tuple>
#include <vector>

#include "figure/factory_shape.hpp"
#include "primitive/color.hpp"
#include "primitive/line.hpp"

void Skeleton::update(double angle, double length_of_base,
    primitive::Point top_point, primitive::Point down_point, primitive::Point pz,
    double block_hypotenuse, bool symmetrical, bool random_color)
{
  double blockSize = sqrt(pow(block_hypotenuse, 2)/2);
  double length = primitive::Line{top_point, down_point}.length();
  double Cx, Cy;
  std::tie(Cx, Cy) = getXYOffsetOnVector(top_point, down_point, blockSize);

  double littleHypotenuse = sqrt(pow(Cx, 2) + pow(Cy, 2));
  int index = 0;
  primitive::Point px1, px2;
  double Vx, Vy;
  double ribLength;
  figure::FactoryShape factory{renderer_};
  while (length >= 0){
      if (random_color){
          factory.color(color_generator_.get());
      }
      primitive::Point vertebra{top_point.x - Cx*index, top_point.y - Cy*index};
      putSquareOnPoint(vertebra, block_hypotenuse, angle);

      length -= littleHypotenuse;
      ++index;

      std::tie(px1, px2) = getPerpendicularLineByPoint(vertebra, top_point, pz, length_of_base);
      std::tie(Vx, Vy) = getXYOffsetOnVector(px1, px2, blockSize);

      ribLength = primitive::Line{px1, px2}.length();
      int vIndex = 1;
      while (ribLength >= 0){
          if (random_color){
              factory.color(color_generator_.get());
          }
          primitive::Point tmpVertebraRight{px1.x - Vx*vIndex, px1.y - Vy*vIndex};
          putSquareOnPoint(tmpVertebraRight, block_hypotenuse, angle);
          if (symmetrical){
              primitive::Point tmpVertebraLeft{px1.x + Vx*vIndex, px1.y + Vy*vIndex};
              putSquareOnPoint(tmpVertebraLeft, block_hypotenuse, angle);
          }
          ribLength -= littleHypotenuse;
          ++vIndex;
      }
  }
}

std::pair<double, double> Skeleton::getXYOffsetOnVector(primitive::Point px1,
                                                        primitive::Point px2,
                                                        double offset_length) const {
    double length = primitive::Line{px1, px2}.length();
    double Cx = (px1.x - px2.x) / (length/(offset_length*2));
    double Cy = (px1.y - px2.y) / (length/(offset_length*2));
    return std::make_pair(Cx, Cy);
}

void Skeleton::fillRect(primitive::Point a, primitive::Point b, primitive::Point c) {
    double Cx_ab, Cy_ab;
    double step = 1;
    double sideLength = primitive::Line{a, b}.length();
    std::tie(Cx_ab, Cy_ab) = getXYOffsetOnVector(a, b, step/2);
    int index = 1;
    figure::FactoryShape factory{renderer_};
    while (sideLength >= 0){
        factory.line({a.x - Cx_ab*index, a.y - Cy_ab*index},
                     {c.x - Cx_ab*index, c.y - Cy_ab*index}).draw();
        sideLength -= step;
        ++index;
    }
}

void Skeleton::putSquareOnPoint(primitive::Point center_point,
                                double block_hypotenuse_length, double angle) {
    std::vector<primitive::Point> littleSqare;
    primitive::Point tmp = center_point;
    tmp.y -= block_hypotenuse_length;

    for (int i = 0; i < 4; ++i){
        double theta = M_PI/4 + i*M_PI/2 - angle; // getTiltAngel();
        auto p = tmp;
        littleSqare.push_back(std::move(p.rotate(center_point, theta)));
    }
    auto iter = littleSqare.begin();
    figure::FactoryShape factory{renderer_};
    for (; iter != littleSqare.end() - 1; ++iter){
        factory.line({iter->x, iter->y},
                     {(iter+1)->x, (iter+1)->y}).draw();
    }
    factory.line({iter->x, iter->y},
                 {littleSqare.begin()->x, littleSqare.begin()->y}).draw();
    fillRect(littleSqare[1], littleSqare[2], littleSqare[0]);
}

std::pair<primitive::Point, primitive::Point>
Skeleton::getPerpendicularLineByPoint(primitive::Point px,
    primitive::Point tp1, primitive::Point tp2, double length_of_base) {
    double Cx, Cy;
    double angle = M_PI_2;

    std::tie(Cx, Cy) = getXYOffsetOnVector(px, tp1, length_of_base); // to make sure pz will be found

    primitive::Point px2{tp1.x - Cx, tp1.y - Cy};
    auto pz = primitive::Line{px, px2.rotate(px, angle)}.intersect({tp1, tp2});

    return std::make_pair(px, pz);
}
