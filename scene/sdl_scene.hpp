#ifndef SCENE_SDL_SCENE_HPP_
#define SCENE_SDL_SCENE_HPP_

#include "primitive/size.hpp"
#include "scene.hpp"

struct SDL_Renderer;

namespace scene {

class SdlScene: public Scene {
public:
    SdlScene(SDL_Renderer* renderer, primitive::Size size)
        : renderer_{renderer}, size_{std::move(size)} {}

    SdlScene(SdlScene&&) = delete;
    SdlScene& operator=(SdlScene&&) = delete;
    SdlScene(SdlScene const&) = delete;
    SdlScene& operator=(SdlScene const&) = delete;
    ~SdlScene() = default;

    void update() override;
    void draw(space::Background const& background) override;
    void draw(space::Grid const& grid) override;
    void draw(space::Ship const& ship) override;
    void draw(space::Nozzle const& nozzle) override;
    void draw(space::Projectile const& projectile) override;
    void draw(space::LifeAmount const& lifes) override;
    void draw(space::Asteroid const& asteroid) override;
    void draw(space::Explosion const& explosion) override;
    void draw(space::Fragment const& fragment) override;

private:
    SDL_Renderer* renderer_{nullptr};
    primitive::Size size_;
};

}  // namespace scene

#endif /* SCENE_SDL_SCENE_HPP_ */
