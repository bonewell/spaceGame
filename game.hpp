#ifndef GAME_H
#define GAME_H

#include <atomic>
#include <exception>
#include <list>
#include <mutex>
#include <vector>

#include "primitive/size.hpp"
#include "primitive/time.hpp"
#include "scene/sdl_scene.hpp"
#include "space/asteroid.hpp"
#include "space/explosion.hpp"
#include "space/background.hpp"
#include "space/life_amount.hpp"
#include "space/projectile.hpp"
#include "space/ship.hpp"

struct SDL_Renderer;

static std::exception_ptr globalExceptionPtr = nullptr;

class GameOverException: public std::exception {
    const char *file_{nullptr};
    int line_{0};
    const char* func_{nullptr};
    const char* info_{nullptr};
public:
    GameOverException(const char *msg, const char *file, int line, const char *func, const char *info=""):
        file_(file),
        line_(line),
        func_(func),
        info_(info)
    {}
    GameOverException(){}
    const char * get_file() {return file_;}
    int get_line() {return line_;}
    const char * get_func() {return func_;}
    const char * get_info() {return info_;}
};

class Game{
public:
    Game(scene::Scene& scene, primitive::Size, int);
    void run();

private:
    scene::Scene& scene_;

    primitive::Size screen_size_;
    space::LifeAmount life_amount_;
    std::atomic_bool running_{false};

    primitive::Time change_position_delay_;
    primitive::Time inertia_delay_;
    primitive::Time update_asteroids_delay_;

    std::mutex asteroids_mutex_;
    std::mutex projectiles_mutex_;

    space::Background background_;
    space::Ship ship_;
    std::list<space::ProjectilePtr> projectiles_;
    std::list<space::AsteroidPtr> asteroids_;
    std::list<space::ExplosionPtr> explosions_;

    // Continuous buttom pushing flags
    bool space_pushed_{false};
    bool left_pushed_{false};
    bool right_pushed_{false};
    bool up_pushed_{false};
    bool up_unpushed_{false};
    bool down_pushed_{false};
    bool down_unpushed_{false};

    int inertia_counter_up_{0};
    int inertia_counter_down_{0};

    void displayObjects();
    void changeObjectsPositions();

    void createAsteroid();
    void updateAsteroids();
    void updateProjectiles();

    void checkHits();
    void histLoop();
    void cleanAsteroids();
    void cleanProjectiles();
    void cleanExplosions();
    void cleanLoop();

    void checkShipHits();
    void shipHitsLoop();
    void update();

    void generateExplosion(space::Asteroid*);
};

#endif
