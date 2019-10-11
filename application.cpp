#include "application.hpp"

#include <cstdlib>
#include <ctime>

#include <SDL2/SDL.h>

#include "game.hpp"
#include "scene/sdl_scene.hpp"

Application::Application(primitive::Size size)
    : size_{std::move(size)}
{
    std::srand(unsigned(std::time(0)));
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        throw SdlError{"Could not initialize SDL"};
    }
}

void Application::run()
{
    auto window = SDL_CreateWindow("Space game",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        size_.width, size_.height, SDL_WINDOW_SHOWN);
    if (!window) {
        throw SdlError{"Could not create window"};
    }

    auto renderer = SDL_CreateRenderer(window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer){
        throw SdlError{"Could not create render"};
    }

    {
        scene::SdlScene scene{renderer, size_};
        // TODO: Extract this dependence
        Game{scene, size_, 5}.run();
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

Application::~Application()
{
  SDL_Quit();
}
