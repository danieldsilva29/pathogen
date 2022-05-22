#include "BESDL.hpp"
#include <cstdio>
#include <ctime>
#include <vector>
#include "Utils.hpp"

using namespace std;

#define SPEED 10 //Speed of projectiles per second
#define DAMAGE 10 //Damage per projectile hit
#define MAXHEALTH 10 //Max health

#define DEG(x) x*(180/M_PI)
#define RAD(x) (x/(float)180)*(float)M_PI

vector<tuple<shared_ptr<ShapeObject>, bool, float>> activeProjectiles;

void shootStuff(App *app, tuple<int, int, int, int> shooter, float rotation, bool isPlayer = false) {
    auto newProjectile = isPlayer ? app->addTexture("bluelaser.png"): app->addTexture("redlaser.png");
    newProjectile->rotation = 270-rotation;
    newProjectile->setCoords(get<0>(shooter) + get<2>(shooter), get<1>(shooter) + get<3>(shooter));
    activeProjectiles.push_back({newProjectile, isPlayer, rotation});
    cout << "sdf" << endl;
}

float xatan (float y, float x) {
    float degrees = DEG(atan2(y, x)); 
    if (degrees < 0) {
        degrees += 360; 
    }
    return degrees;
}

int main () {
    // 1000 by 1000 window
    App app = App("Pathogen?????", 800, 800);

    // Create a new enemy
    vector<tuple<shared_ptr<ShapeObject>, int>> enemies;
    int enemy_size = 10;
    for (int i = 0; i < 10; i++) {
        int direction = rand() % 360;
        float range = (rand() % 20000) / (float) 100 + 200;

        float radians = RAD(direction);
        float x = cos(radians);
        float y = sin(radians);
        x = (app.window_width / 2) + range * x;
        y = (app.window_height / 2) + range * y;

        auto enemy = app.addTexture("enemyship1.png");
        enemies.push_back({enemy, MAXHEALTH});
        enemy->rotation = 90 - direction;

        enemy->setCoords(x, y, 100, 100);

    }

    //create player texture
    int velocity_player = 0;
    auto player = app.addTexture("spaceship.png");
    player->setCoords(500, 500, 100, 200);
    
    bool close = false;
    
    clock_t time = clock();
 
    while (!close) {
        // Events management
        auto [pressed_key, mouseX, mouseY, did_click, should_close] = app.getInteraction();
        if (should_close) {
            close = true;
        }
    
        // Enemy follow player
        if (enemy_size > enemies.size()) {
            enemy_size = enemies.size();
            cout << "enemy size: " << enemies.size() << endl;
        }
        for (auto enemy : enemies) {
            auto rectangle = get<0>(enemy);
            int deltaY = (player->getCoords(COORD_Y) + player->getCoords(COORD_HEIGHT) / 2) - rectangle->getCoords(COORD_Y);
            int deltaX = (player->getCoords(COORD_X) + player->getCoords(COORD_WIDTH) / 2) - rectangle->getCoords(COORD_X);
            float rotation = xatan(deltaY, deltaX); 
            rectangle->rotation = 270 - rotation;
            
            // cout << "Player X:" << player->x + player->w / 2 << " Player X: " << player->y + player->h / 2 << " Rotation: " << rotation << endl;

            // enemy move towards player
            rectangle->getCoords(COORD_X) += 2 * cos(RAD(rotation));
            rectangle->getCoords(COORD_Y) += 2 * sin(RAD(rotation));
        }
    
        // Player follow mouse
        int deltaY = mouseY - (player->getCoords(COORD_Y) + player->getCoords(COORD_HEIGHT) / 2);
        int deltaX = mouseX - (player->getCoords(COORD_HEIGHT) + player->getCoords(COORD_HEIGHT) / 2);
        float rotation = xatan(deltaY, deltaX); 
        player->rotation = 270-rotation;

        switch (pressed_key) {
            case 'w':
                player->getCoords(COORD_X) += 10 * cos(RAD(rotation));
                player->getCoords(COORD_Y) += 10 * sin(RAD(rotation)); 

                break;
        }
        
        if (did_click) {
            shootStuff(&app, player->getCoords(), rotation, true);
        }
        
        int counter = 0;
        bool wasCollided = false;
        for (auto &[projectile, ___useless___, rot] : activeProjectiles) {
            for (auto enemy : enemies) {
                int enemyCounter = 0;
                if (projectile->hasIntersection(get<0>(enemy))) { 
                    get<1>(enemy) -= 10;
                    if (get<1>(enemy) <= 0) {
                        cout << "[" << clock() << "] Beshan is dog" << endl;
                        VECTOR_REMOVE(enemies, enemyCounter);
                    }
                    VECTOR_REMOVE(activeProjectiles, counter);
                    wasCollided = true;
                }
                ++enemyCounter;
            }
            if (!wasCollided) {
                float seconds = (clock() - time) / 1000;
                projectile->getCoords(COORD_X) += seconds * SPEED * cos(RAD(rot));
                projectile->getCoords(COORD_Y) += seconds * SPEED * sin(RAD(rot));
                ++counter;
                wasCollided = false;
            }
        }
        time = clock();
        app.render();

    }
    
}
