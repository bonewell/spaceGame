#include "sdl_scene.hpp"

#include <cmath>

#include "colorschema.hpp"
#include "figure/factory_shape.hpp"
#include "primitive/color.hpp"
#include "primitive/point.hpp"
#include "primitive/size.hpp"
#include "skeleton.hpp"
#include "space/asteroid.hpp"
#include "space/background.hpp"
#include "space/explosion.hpp"
#include "space/grid.hpp"
#include "space/life_amount.hpp"
#include "space/projectile.hpp"
#include "space/nozzle.hpp"
#include "space/ship.hpp"

namespace scene {

void SdlScene::update()
{
    SDL_RenderPresent(renderer_);
    SDL_RenderClear(renderer_);
}

void SdlScene::draw(space::Background const& background)
{
    constexpr auto kCitizenSize = 25;

    const auto& start = background.gradient_start;
    const auto& finish = background.gradient_finish;

    int steps = (size_.width / kCitizenSize) + 1;
    int r_step = (finish.red - start.red) / steps;
    int g_step = (finish.green - start.green) / steps;
    int b_step = (finish.blue - start.blue) / steps;

    primitive::Color color = start;
    figure::FactoryShape factory{renderer_};
    for (auto i = 0; i < steps; ++i) {
        color.red += r_step;
        color.green += g_step;
        color.blue += b_step;

        factory.color(color)
               .rectangle({0.0, double(kCitizenSize * i)},
                          {size_.width, kCitizenSize})
               .fill();
    }
    if (background.grid) background.grid->display(*this);
}

void SdlScene::draw(space::Grid const& grid)
{
    figure::FactoryShape factory{renderer_};
    factory.color(grid.color);
    auto i{0};
    while (i < size_.height || i < size_.width) {
        factory.line({0.0, double(i)},
                     {double(size_.width), double(i)}).draw();
        factory.line({double(i), 0.0},
                     {double(i), double(size_.height)}).draw();
        i += grid.cell_size;
    }
}

void SdlScene::draw(space::Ship const& ship)
{
    figure::FactoryShape factory{renderer_};
    const auto& border = ship.get_border();
    factory.color(ship.colors.get_color()).polygon(border).draw();

    Skeleton skeleton{renderer_, ColorGenerator{{220,220,220}, {192,192,192},
                                                {105,105,105}, {211,211,211},
                                                {119,136,153}}};
    skeleton.update(ship.getTiltAngel(), size_.width, border[0],
                   primitive::Point{border[1].x/2 + border[2].x/2,
                                    border[1].y/2 + border[2].y/2},
                   border[2], 4, true, true);
    ship.get_left_nozzle().display(*this);
    ship.get_right_nozzle().display(*this);
}

void SdlScene::draw(space::Nozzle const& nozzle)
{
    figure::FactoryShape factory{renderer_};
    const auto& border = nozzle.get_border();
    factory.color(nozzle.colors.get_color()).polygon(border).draw();

    Skeleton skeleton{renderer_, ColorGenerator{{220,220,220}, {192,192,192},
                                                {105,105,105}, {211,211,211},
                                                {119,136,153}}};
    skeleton.update(nozzle.getTiltAngel(), size_.width, border[0],
                    primitive::Point{border[1].x/2 + border[2].x/2,
                                     border[1].y/2 + border[2].y/2},
                    border[2], 1, true, false);
}

void SdlScene::draw(space::Projectile const& projectile)
{
    double error = double(-projectile.kBlockSize);
    double tmp_x{projectile.kBlockSize - 0.5};
    double tmp_y{0.5};

    const auto& c = projectile.get_coordinate();

    double cx = c.x - 0.5;
    double cy = c.y - 0.5;

    figure::FactoryShape factory{renderer_};
    factory.color({255, 255, 0, 255});
    while (tmp_x >= tmp_y){
        factory.point({cx + tmp_x, cy + tmp_y}).draw();
        factory.point({cx + tmp_y, cy + tmp_x}).draw();

        if (tmp_x != 0){
            factory.point({cx - tmp_x, cy + tmp_y}).draw();
            factory.point({cx + tmp_y, cy - tmp_x}).draw();
        }

        if (tmp_y != 0){
            factory.point({cx + tmp_x, cy - tmp_y}).draw();
            factory.point({cx - tmp_y, cy + tmp_x}).draw();
        }

        if (tmp_x != 0 && tmp_y != 0){
            factory.point({cx - tmp_x, cy - tmp_y}).draw();
            factory.point({cx - tmp_y, cy - tmp_x}).draw();
        }

        error += tmp_y;
        ++tmp_y;
        error += tmp_y;

        if (error >= 0){
            --tmp_x;
            error -= tmp_x;
            error -= tmp_x;
        }
    }

    factory.color({128, 0, 128, 255});
    for (double dy = 1; dy <= projectile.kBlockSize; dy += 1.0){
        double dx = std::floor(std::sqrt((2.0 * projectile.kBlockSize * dy) - (dy * dy)));
        factory.line({cx - dx, cy + dy - projectile.kBlockSize},
                     {cx + dx, cy + dy - projectile.kBlockSize}).draw();
        factory.line({cx - dx, cy - dy + projectile.kBlockSize},
                     {cx + dx, cy - dy + projectile.kBlockSize}).draw();
    }
}

void SdlScene::draw(space::LifeAmount const& lifes)
{
    figure::FactoryShape factory{renderer_};
    factory.color({0, 255, 0, 255})
        .rectangle({5.0, double(size_.height - 32)}, lifes.kSize).fill();

    factory.color({255, 0, 0, 255});
    for (int i = 0; i < lifes.get_amount(); ++i) {
        factory.rectangle({10.0 + i * 15, double(size_.height - 30)},
                          lifes.kBarSize).fill();
    }
}

void SdlScene::draw(space::Asteroid const& asteroid)
{
    const auto& points = asteroid.get_points();
    primitive::Point p1;
    primitive::Point p2;
    figure::FactoryShape factory{renderer_};
    factory.color({0, 0, 255, 255});
    auto iter = std::begin(points);
    for (; iter != std::end(points)-1; ++iter) {
        p1 = *iter;
        p2 = *(iter+1);
        factory.line(p1, p2).draw();
    }
    p1 = *std::begin(points);
    factory.line(p1, p2).draw();

    auto center_point = asteroid.center();
    int blocksize = 3;

    iter = std::begin(asteroid.get_points());
    auto iter_next = iter + 1;

    p1 = *iter;
    p2 = *iter_next;

    Skeleton skeleton{renderer_, asteroid.colors};
    while (iter_next != std::end(asteroid.get_points())) {
        p1 = *iter;
        p2 = *iter_next;
        skeleton.update(0.0, primitive::Line{p1, p2}.length(), center_point,
                        primitive::Point{(p1.x + p2.x)/2, (p1.y + p2.y)/2}, p1,
                        blocksize, false, true);
        ++iter;
        ++iter_next;
    }
    iter_next = std::begin(asteroid.get_points());
    p1 = *iter;
    p2 = *iter_next;
    skeleton.update(0.0, primitive::Line{p1, p2}.length(), center_point,
                    primitive::Point{(p1.x + p2.x)/2, (p1.y + p2.y)/2}, p1,
                    blocksize, false, true);
}

void SdlScene::draw(space::Explosion const& explosion)
{
    for (const auto& part: explosion.fragments()) {
        part.display(*this);
    }
}

void SdlScene::draw(space::Fragment const& fragment)
{
  int blocksize = 3;
  primitive::Point p1 = *(fragment.dots.begin());
  primitive::Point p2 = *(fragment.dots.begin() + 1);
  primitive::Point p3 = *(fragment.dots.begin() + 2);
  Skeleton{renderer_, fragment.colors}.update(0.0, primitive::Line{p3, p2}.length(),
      p2, primitive::Point{(p1.x + p2.x)/2, (p1.y + p2.y)/2}, p1, blocksize, false, true);
}

}  // namespace scene

