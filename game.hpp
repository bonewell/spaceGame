#ifndef GAME_H
#define GAME_H

#include <atomic>
#include <list>
#include <mutex>
#include <thread>

#include "primitive/size.hpp"
#include "primitive/time.hpp"
#include "scene/sdl_scene.hpp"
#include "space/asteroid.hpp"
#include "space/explosion.hpp"
#include "space/background.hpp"
#include "space/life_amount.hpp"
#include "space/projectile.hpp"
#include "space/ship.hpp"

struct Action {
    bool space_pushed{false};
    bool left_pushed{false};
    bool right_pushed{false};
    bool up_pushed{false};
    bool up_unpushed{false};
    bool down_pushed{false};
    bool down_unpushed{false};
};

class Game {
public:
    Game(scene::Scene& scene, primitive::Size, int);
    void start();
    void stop();
    void display();
    bool update(Action const& action);

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
    std::mutex explosions_mutex_;
    std::thread hits_monitoring_;
    std::thread ship_hits_monitoring_;

    space::Background background_;
    space::Ship ship_;
    std::list<space::ProjectilePtr> projectiles_;
    std::list<space::AsteroidPtr> asteroids_;
    std::list<space::ExplosionPtr> explosions_;

    int inertia_counter_up_{0};
    int inertia_counter_down_{0};

    void changeObjectsPositions();

    void createAsteroid();
    void moveAsteroids(primitive::Direction d);
    void updateAsteroids();
    void displayAsteroids();
    void cleanAsteroids();

    void checkHits();
    void histLoop();

    void createProjectile();
    void moveProjectiles(primitive::Direction d);
    void updateProjectiles();
    void displayProjectiles();
    void cleanProjectiles();

    void createExplosion(space::Asteroid*);
    void moveExplosions(primitive::Direction d);
    void displayExplosions();
    void cleanExplosions();

    void checkShipHits();
    void shipHitsLoop();
};

#endif
