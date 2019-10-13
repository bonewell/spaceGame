#ifndef APPLICATION_HPP_
#define APPLICATION_HPP_

#include <stdexcept>

#include "primitive/size.hpp"

struct SDL_Renderer;

class SdlError : public std::runtime_error {
public:
  using std::runtime_error::runtime_error;
};

class Application {
public:
    explicit Application(primitive::Size size);
    ~Application();

    Application(Application&&) = delete;
    Application& operator=(Application&&) = delete;
    Application(Application const&) = delete;
    Application& operator=(Application const&) = delete;

    void run();

private:
    primitive::Size size_;
    SDL_Renderer* renderer_{nullptr};
};

#endif /* APPLICATION_HPP_ */
