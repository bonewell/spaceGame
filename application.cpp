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

    scene::SdlScene scene{renderer, size_};
    // TODO: Extract this dependence
    Game game{scene, size_, 5};
    game.start();
    auto running{true};
    Action action;
    SDL_Event event;
    while (running) {
        game.display();
        while (SDL_PollEvent(&event) != 0) {
            if(event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_KEYDOWN) {
                switch(event.key.keysym.sym){
                    case SDL_QUIT:
                        running = false;
                        break;
                    case SDLK_UP:
                        action.up_pushed = true;
                        action.up_unpushed = false;
                        break;
                    case SDLK_DOWN:
                        action.down_pushed = true;
                        action.down_unpushed = false;
                        break;
                    case SDLK_LEFT:
                        action.left_pushed = true;
                        break;
                    case SDLK_RIGHT:
                        action.right_pushed = true;
                        break;
                    case SDLK_SPACE:
                        action.space_pushed = true;
                        break;
                    default:
                        break;
                }
            } else if (event.type == SDL_KEYUP) {
                switch(event.key.keysym.sym){
                    case SDLK_UP:
                        action.up_pushed = false;
                        action.up_unpushed = true;
                        break;
                    case SDLK_DOWN:
                        action.down_pushed = false;
                        action.down_unpushed = true;
                        break;
                    case SDLK_LEFT:
                        action.left_pushed = false;
                        break;
                    case SDLK_RIGHT:
                        action.right_pushed = false;
                        break;
                    case SDLK_SPACE:
                        action.space_pushed = false;
                        break;
                    default:
                        break;
                }
            }
        }
        running = running && game.update(action);
    }
    game.stop();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

Application::~Application()
{
    SDL_Quit();
}
