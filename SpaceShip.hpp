#ifndef SPACESHIP_H
#define SPACESHIP_H

#include "SDL2/SDL.h"
#include <functional>
#include <vector>
#include <iostream>
#include <chrono>
#include <time.h>
#include "speed.hpp"
#include <utility>
#include "colorschema.hpp"
#include "common.hpp"

#define BLOCK_SIZE 5
#define POINTS_COUNT 4
#define ASTEROID_POINTS_COUNT 7
#define ROTATION_DELAY 60
#define DISPLAY_DELAY 10
#define SHOOTING_DELAY 100
#define CHANGE_POSITION_DELAY 30
#define INERTIA_DELAY 10
#define INERTIA_COUNTER 500
#define PROJ_LIFETIME 5000
#define SHIP_COLOR_CHANGE 50

using namespace std;

struct Nozzle{
    vector <Point> points;
    vector <Point> originPoints;
    SDL_Renderer* renderer;
    ColorSchema *cs;
    Speed *speed;

    Nozzle(SDL_Renderer* renderer, Point a, Point b, Point c, Speed *speed){
        originPoints.push_back(a);
        originPoints.push_back(b);
        originPoints.push_back(c);
        copy(originPoints.begin(), originPoints.end(), back_inserter(points));
        this->renderer = renderer;
        this->speed = speed;
        cs = new ColorSchema(Color(255, 17, 0), Color(255, 237, 0));
    }
    void display(){
        cs->update(speed->getCurrentA());
        SDL_SetRenderDrawColor(renderer, cs->getR(), cs->getG(), cs->getB(), 255);
        SDL_RenderDrawLine(renderer, points[0].x, points[0].y, points[1].x, points[1].y);
        SDL_RenderDrawLine(renderer, points[0].x, points[0].y, points[2].x, points[2].y);
        SDL_RenderDrawLine(renderer, points[1].x, points[1].y, points[2].x, points[2].y);
    }


    void update(){
        double offsetLength = speed->getCurrentA();
        points.clear();
        copy(originPoints.begin(), originPoints.end(), back_inserter(points));

        if (offsetLength < 0.001) { return; }

        double Cx = (points[0].x - points[1].x) * (offsetLength);
        double Cy = (points[0].y - points[1].y) * (offsetLength);

        double Cx2 = (points[1].x - points[2].x) * (offsetLength);
        double Cy2 = (points[1].y - points[2].y) * (offsetLength);

        points[0].x += Cx;
        points[0].y += Cy;

        points[2].x -= Cx2;
        points[2].y -= Cy2;
    }
};

class SpaceObject{
protected:
    SDL_Renderer *renderer;
	int x, y;
	int screen_width;
	int screen_height;
	std::chrono::time_point<std::chrono::system_clock> display_delay; 
	std::chrono::time_point<std::chrono::system_clock> rotation_delay;
    bool alive;
public:
	SpaceObject(SDL_Renderer*, int, int, int, int);
	virtual void display(bool) = 0;
	virtual void change_position(DirectionXY);
    virtual ~SpaceObject();
    bool isAlive();
    void markAsDead();
};

class Projectile: public SpaceObject{
private:
	int direction_x;
	int direction_y;
    double x_previous;
    double y_previous;
    std::chrono::time_point<std::chrono::system_clock> life_time;
public:
    Projectile(SDL_Renderer*, int, int, int, int, int, int);
	void change_position(DirectionXY);
    void display(bool);
    Point* getXY();
    pair<Point, Point> getLine();
    std::chrono::time_point<std::chrono::system_clock> getLifeTime();
    ~Projectile();
};

class SpaceShip: public SpaceObject{
private:
    ColorSchema *cs;
    vector<Point> skeleton;
    std::chrono::time_point<std::chrono::system_clock> shoot_delay;
    std::chrono::time_point<std::chrono::system_clock> ship_color_change;
    Point initialMedianIntersection;
    double getTiltAngel();
    double getLengthOfBase();
    int spaceWidth;
    int spaceHeight;
    double nozzleMinHeight;
    double nozzleMaxHeight;
    int nozzleWidth;
    Nozzle *leftNozzle;
    Nozzle *rightNozzle;
public:
    colorGeneratorShip *cg;
    vector<Point> pp;
    void updateNozzles();
    double getCurrentA();
    Point getMedianIntersaction();
    Speed *speed;
    void slowdown();
    void accelarate();
    void backward_slowdown();
    void backward_accelarate();
    SpaceShip(SDL_Renderer*, int, int, int);
	Projectile * shoot();
	vector<Projectile*> projectiles;
	DirectionXY get_direction();
    DirectionXY get_offset();
	void display(bool);
	void change_x(bool);
	void change_y(bool);
    DirectionalVector getDerectionalVector();
    ~SpaceShip();
};

class Asteroid: public SpaceObject{
private:
    vector<Point*> pp;
    void fill();
public:
    colorGeneratorAsteroid *cg;
    Point getCenterPoint();
	void display(bool);
    vector<Point*>& getPoints();
	Asteroid(SDL_Renderer*, int, int, int, int);
	void change_position(DirectionXY);
    Point* getFirstPoint();
    ~Asteroid();
};

#endif
