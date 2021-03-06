#include "Game.hpp"

Game::Game(SDL_Renderer *renderer, int screen_width, int screen_height, int liveAmount){

    this->liveAmount = liveAmount;

    this->screen_width = screen_width;
    this->screen_height = screen_height;

    // Hope I will think something more interesting background than it is now
    my_background = new Background(renderer, screen_width, screen_height);

    // Renderer
    this->renderer = renderer;

    // Create ship
    my_ship = new SpaceShip(renderer, screen_width, screen_height, 50);

    // Create asteroids
    for (int i = 0; i < 10; ++i) {
        int tmp_x = rand() % this->screen_width;
        int tmp_y = rand() % this->screen_height;
        asteroids.push_back(new Asteroid(renderer, screen_width, screen_height, tmp_x, tmp_y));
    }

    // Initiating delays
    change_position_delay = NOW + static_cast<std::chrono::milliseconds> (CHANGE_POSITION_DELAY);
    inertia_delay = NOW + static_cast<std::chrono::milliseconds> (INERTIA_DELAY);
    update_asteroids_delay = NOW + static_cast<std::chrono::milliseconds> (ASTEROIDS_REMOVING_DELAY);

    space_pushed 	= false;
    left_pushed  	= false;
    right_pushed 	= false;
    up_pushed 	= false;
    down_pushed 	= false;
    up_unpushed     = false;
    down_unpushed   = false;

    inertia_counter_up = 0;
    inertia_counter_down = 0;
}

void Game::create_asteroid(){
    double theta = (rand() % 360)/M_PI;
    Point ship_center = my_ship->getMedianIntersaction();
    double tmp_x = (rand() % this->screen_width) + this->screen_width;
    double tmp_y = ship_center.y;

    point P(tmp_x, tmp_y);
    point Q(ship_center.x, ship_center.y);
    point P_rotated = (P-Q) * polar(1.0, theta) + Q;
    tmp_x = P_rotated.real();
    tmp_y = P_rotated.imag();

    asteroids.push_back(new Asteroid(renderer, screen_width, screen_height, tmp_x, tmp_y));
}

void Game::update_asteroids(){
    if (update_asteroids_delay >= NOW){ return; }
    Point tmp_p = my_ship->getMedianIntersaction();
    Point *ap = nullptr;
    double distance;
    double diagonal = sqrt(pow(this->screen_width, 2) + pow(this->screen_height, 2));
    auto iter = asteroids.begin();
    while (iter != asteroids.end())
    {
        ap = dynamic_cast<Asteroid*>(*iter)->getFirstPoint();
        distance = sqrt(pow(tmp_p.x - ap->x, 2) + pow(tmp_p.y - ap->y, 2));
        if (distance >= 1.5 * diagonal){
            SpaceObject *tmp = *iter;
            asteroids.erase(iter++);
            delete tmp;
        } else {
            ++iter;
        }
    }
    update_asteroids_delay = NOW + static_cast<std::chrono::milliseconds> (ASTEROIDS_REMOVING_DELAY);
}

void Game::update_projectiles(){
    auto iter = projectiles.begin();
    while (iter != projectiles.end())
    {
        if (dynamic_cast<Projectile*>(*iter)->getLifeTime() < NOW){
            SpaceObject *tmp = *iter;
            projectiles.erase(iter++);
            delete tmp;
        } else {
            ++iter;
        }
    }
}

bool dot_on_line(Point *p1, Point *p2, Point *px)
{
    double k = (p2->y - p1->y) / (p2->x - p1->x);
    double c = p1->y - k * p1->x;
    return abs(px->y - (px->x * k + c)) < 0.1;
}

inline int det (int a, int b, int c, int d) {
    return a * d - b * c;
}

inline bool between (int a, int b, double c) {
    double epsilon = 1E-9;
    return min(a,b) <= c + epsilon && c <= max(a,b) + epsilon;
}

inline bool intersect_1 (int a, int b, int c, int d) {
    if (a > b)  swap (a, b);
    if (c > d)  swap (c, d);
    return max(a,c) <= min(b,d);
}


bool intersect (Point a, Point b, Point c, Point d) {
    int A1 = a.y-b.y,  B1 = b.x-a.x,  C1 = -A1*a.x - B1*a.y;
    int A2 = c.y-d.y,  B2 = d.x-c.x,  C2 = -A2*c.x - B2*c.y;
    int zn = det (A1, B1, A2, B2);
    if (zn != 0) {
        double x = - det (C1, B1, C2, B2) * 1. / zn;
        double y = - det (A1, C1, A2, C2) * 1. / zn;
        return between (a.x, b.x, x) && between (a.y, b.y, y)
            && between (c.x, d.x, x) && between (c.y, d.y, y);
    }
    else
        return det (A1, C1, A2, C2) == 0 && det (B1, C1, B2, C2) == 0
            && intersect_1 (a.x, b.x, c.x, d.x)
            && intersect_1 (a.y, b.y, c.y, d.y);
}

void Game::check_ship_hits(){
    while (true){
        asteroids_mutex.lock();
        ship_hits_loop();
        asteroids_mutex.unlock();
        usleep(100);
    }
}

void Game::ship_hits_loop(){

    vector<Point*> tmpPoints;
    bool hitStatus = false;

    for (auto ast = asteroids.begin(); ast != asteroids.end(); ++ast){
        hitStatus = false;
        if (!(*ast)->isAlive()){ continue; }
        tmpPoints = dynamic_cast<Asteroid*>(*ast)->getPoints();
        Point *p1, *p2;
        Point sp1, sp2;
        for (auto p = tmpPoints.begin(); p != tmpPoints.end() - 1; ++p){
            if (!hitStatus){
                p1 = *p;
                p2 = *(p+1);
                if (tmpPoints.end()-1 == p){
                    p2 = *tmpPoints.begin();
                } else {
                    p2 = *(p + 1);
                }
                for (auto spacePointsIter = my_ship->pp.begin(); spacePointsIter != my_ship->pp.end() - 1; ++spacePointsIter){
                    sp1 = *spacePointsIter;
                    sp2 = *(spacePointsIter+1);
                    if (my_ship->pp.end()-1 == spacePointsIter){
                        sp2 = *my_ship->pp.begin();
                    } else {
                        sp2 = *(spacePointsIter + 1);
                    }
                    hitStatus = intersect(*p1, *p2, sp1, sp2);
                    if (hitStatus){
                        liveAmount--;
                        if (!liveAmount){
                            try{
                                throw GameOverException();
                            }catch (...){
                                globalExceptionPtr = std::current_exception();
                            }
                        }
                        (*ast)->markAsDead();
                        break;
                    }
                }
            }
        }
    }
    clean_asteroids();
}

void Game::check_hits(){
    while (true)
    {
        projectiles_mutex.lock();
        asteroids_mutex.lock();
        hist_loop();
        asteroids_mutex.unlock();
        projectiles_mutex.unlock();
        usleep(100);
    }
}

void Game::hist_loop(){

    vector<Point*> tmpPoints;
    bool hitStatus;

    for (auto pr = projectiles.begin(); pr != projectiles.end(); ++pr){
        if (!(*pr)->isAlive()) { continue; }
        hitStatus = false;
        for (auto ast = asteroids.begin(); ast != asteroids.end(); ++ast){
            if (!(*ast)->isAlive()) { continue; }
            tmpPoints = dynamic_cast<Asteroid*>(*ast)->getPoints();
            Point *p1, *p2, *px;

            for (auto p = tmpPoints.begin(); p != tmpPoints.end() - 1; ++p){
                p1 = *p;
                p2 = *(p+1);
                if (tmpPoints.end() - 1 == p){
                    p2 = *tmpPoints.begin();
                } else {
                    p2 = *(p + 1);
                }

                px = dynamic_cast<Projectile*>(*pr)->getXY();
                pair<Point, Point> pLine = dynamic_cast<Projectile*>(*pr)->getLine();

                bool hitStatus = intersect(*p1, *p2, pLine.first, pLine.second);
                delete px;
                if ( hitStatus ){
                    (*ast)->markAsDead();
                    (*pr)->markAsDead();
                    break;
                }
            }
        }
    }
    clean_loop();
}

void Game::generate_explosion(Asteroid *tmp_ast){
    double middle_x = 0.0, middle_y = 0.0;

    vector<Point*> tmpPoints = tmp_ast->getPoints();
    for (auto iter = tmpPoints.begin(); iter != tmpPoints.end(); ++iter){
        middle_x += (*iter)->x;
        middle_y += (*iter)->y;
    }
    middle_x /= tmpPoints.size();
    middle_y /= tmpPoints.size();
    explosions.push_back(new Explosion(Point(middle_x, middle_y), renderer, tmp_ast));
}

void Game::clean_asteroids(){
    auto ast = asteroids.begin();
    while (ast != asteroids.end()){
        if (!(*ast)->isAlive()){
            Asteroid *tmp_ast = dynamic_cast<Asteroid*>(*ast);
            generate_explosion(tmp_ast);
            asteroids.erase(ast++);
            // delete tmp_ast; // will be remove in ~Explosion() 
        } else {
            ++ast;
        }
    }
}

void Game::clean_projectiles(){
    auto pr = projectiles.begin();
    while (pr != projectiles.end()){
        if (!(*pr)->isAlive()){
            Projectile *tmp_pr = dynamic_cast<Projectile*>(*pr);
            projectiles.erase(pr++);
            delete tmp_pr;
        } else {
            ++pr;
        }
    }
}

void Game::clean_explosions(){
    auto expl = explosions.begin();
    while (expl != explosions.end()){
        if (!(*expl)->isAlive()){
            Explosion *tmp_expl = *expl;
            explosions.erase(expl++);
            delete tmp_expl;
        } else {
            ++expl;
        }
    }
}

void Game::clean_loop(){
    clean_asteroids();
    clean_projectiles();
}

void Game::displayObjects(bool displaySkeletons=false){
	my_ship->display(displaySkeletons);

    for (auto spaceObject = asteroids.begin(); spaceObject != asteroids.end(); ++spaceObject){
        if ((*spaceObject)->isAlive()){
            (*spaceObject)->display(displaySkeletons);
        }
    }
    for (auto spaceObject = projectiles.begin(); spaceObject != projectiles.end(); ++spaceObject){
        if ((*spaceObject)->isAlive()){
            (*spaceObject)->display(displaySkeletons);
        }
    }

    for (auto explosion = explosions.begin(); explosion != explosions.end(); ++explosion){
        if ( (*explosion)->isAlive() ){
            (*explosion)->display(displaySkeletons);
        }
    }
    clean_explosions();
}

void Game::changeObjectsPositions(){

    if (change_position_delay > NOW){ return; }

    DirectionXY directionXY = my_ship->get_offset();

    for (auto spaceObject = asteroids.begin(); spaceObject != asteroids.end(); ++spaceObject){
        (*spaceObject)->change_position(directionXY);
    }
    for (auto spaceObject = projectiles.begin(); spaceObject != projectiles.end(); ++spaceObject){
        (*spaceObject)->change_position(directionXY);
    }
    for (auto explosion = explosions.begin(); explosion != explosions.end(); ++explosion){
        if ( (*explosion)->isAlive() ){
            (*explosion)->shift(directionXY);
        }
    }

    change_position_delay = NOW + static_cast<std::chrono::milliseconds> (CHANGE_POSITION_DELAY);
}

void Game::displayLifeAmount(){
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

    SDL_Rect rect;
    rect.x = 5;
    rect.y = screen_height - 32;
    rect.w = 5 * 15 + 4;
    rect.h = 24;
    SDL_RenderFillRect(renderer, &rect);

    for (int i = 0; i < liveAmount; ++i){
        rect.x = 10 + i * 15;
        rect.y = screen_height - 30;
        rect.w = 10;
        rect.h = 20;
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
    }
}

void Game::run(){

	my_background->fill_background();
    displayObjects();
	SDL_RenderPresent(renderer);
	SDL_RenderClear(renderer);

//    thread thUpdating(&Game::update, this);
    thread thHitsMonitoring(&Game::check_hits, this);
    thread thShipHitsMonitoring(&Game::check_ship_hits, this);

//    thUpdating.join();
    thHitsMonitoring.detach();
    thShipHitsMonitoring.detach();
    int quit = 1;
    SpaceObject *tmp_space_obj;
    while(quit) {

        if (globalExceptionPtr)
          {
            try
            {
              std::rethrow_exception(globalExceptionPtr);
            }
            catch (const std::exception &ex)
            {
              std::cout << "Gema Over!" << endl;
              quit = false;
            }
          }

        projectiles_mutex.lock();
        asteroids_mutex.lock();
        while( SDL_PollEvent( &e ) != 0 ) {
            if( e.type == SDL_QUIT ){
                quit = 0;
            } else if (e.type == SDL_KEYDOWN) {
                switch(e.key.keysym.sym){
                    case SDL_QUIT:
                        quit = 0;
                        break;
                    case SDLK_UP:
                        up_pushed = true;
                        up_unpushed = false;
                        break;
                    case SDLK_DOWN:
                        down_pushed = true;
                        down_unpushed = false;
                        break;
                    case SDLK_LEFT:
                        left_pushed = true;
                        break;
                    case SDLK_RIGHT:
                        right_pushed = true;
                        break;
                    case SDLK_SPACE:{
                            space_pushed = true;
                            tmp_space_obj = my_ship->shoot();
                            if (nullptr != tmp_space_obj) {
                                projectiles.push_back(tmp_space_obj);
                            }
                    }
                        break;
                    default:
                        break;
                }
            } else if (e.type == SDL_KEYUP) {
                switch(e.key.keysym.sym){
                    case SDLK_UP:
                        up_pushed = false;
                        up_unpushed = true;
                        break;
                    case SDLK_DOWN:
                        down_pushed = false;
                        down_unpushed = true;
                        break;
                    case SDLK_LEFT:
                        left_pushed = false;
                        break;
                    case SDLK_RIGHT:
                        right_pushed = false;
                        break;
                    case SDLK_SPACE:
                        space_pushed = false;
                        break;
                    default:
                        break;
                }
            }
        }

        if (up_pushed) 	{ my_ship->backward_accelarate(); my_ship->slowdown(); } else if (up_unpushed) {my_ship->backward_slowdown();}
        if (down_pushed) { my_ship->accelarate(); my_ship->backward_slowdown(); } else if (down_unpushed) {my_ship->slowdown();}

        if (left_pushed) 	{ my_ship->change_x(false); }
        if (right_pushed) 	{ my_ship->change_x(true); }
        {
        if (space_pushed) 	{
            tmp_space_obj = my_ship->shoot();
            if (nullptr != tmp_space_obj) {
                projectiles.push_back(tmp_space_obj);
            }
        }
        changeObjectsPositions();
        update_asteroids();
        update_projectiles();
        if (asteroids.size() <= 20){
            create_asteroid();
        }
        }
        projectiles_mutex.unlock();
        asteroids_mutex.unlock();

        my_background->fill_background();
        displayObjects();
        displayLifeAmount();
        SDL_RenderPresent(renderer);
        SDL_RenderClear(renderer);
    }

}
