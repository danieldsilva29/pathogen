#include "BESDL.hpp"
#include <cstdio>
#include <ctime>
#include <vector>
using namespace std;

#define SPEED 10 //Speed of projectiles per second
#define DAMAGE 10 //Damage per projectile hit
#define MAXHEALTH 10 //Max health

#define DEG(x) x*(180/M_PI)
#define RAD(x) (x/(float)180)*(float)M_PI
#define TODO(x) cout << "Task for Beshan the dog: " << x << endl
#define VECTOR_REMOVE(vec, index) vec.erase(vec.begin()+index)

float xatan (float y, float x) {
    float degrees = DEG(atan2(y, x)); 
    if (degrees < 0) {
        degrees += 360; 
    }
    return degrees;
}

int main () {
    // 1000 by 1000 window
    App app = App("Pathogen?????");

    // Create a new enemy
    vector<tuple<SDL_Rect*, int>> enemies;
    for (int i = 0; i < 10; i++) {
        int direction = rand() % 360;
        float range = (rand() % 20000) / (float) 100 + 200;

        float radians = RAD(direction);
        float x = cos(radians);
        float y = sin(radians);
        x = (app.window_width / 2) + range * x;
        y = (app.window_height / 2) + range * y;

        SDL_Rect* enemy = app.addTexture("enemyship1.png");
        enemies.push_back({enemy, MAXHEALTH});
        app.setRotation(enemy, 90 - direction);

        enemy->x = x;
        enemy->y = y;
        enemy->w = 100;
        enemy->h = 100;

    }

    //create player texture
    int velocity_player = 0;
    SDL_Rect* player = app.addTexture("spaceship.png");
    player->x = 500;
    player->y = 500;
    player->w = 100;
    player->h = 200;
    
    bool close = false;
    vector<tuple<SDL_Rect*, bool, float>> activeProjectiles;
    clock_t time = clock();
    while (!close) {
        // Events management
        auto [pressed_key, mouseX, mouseY, did_click, should_close] = app.getInteraction();
        if (should_close) {
            close = true;
        }
    
        // Enemy follow player
        for (auto enemy : enemies) {
            SDL_Rect* rectangle = get<0>(enemy);
            int health = get<1>(enemy);
            int deltaY = (player->y + player->h / 2) - rectangle->y;
            int deltaX = (player->x + player->w / 2) - rectangle->x;
            float rotation = xatan(deltaY, deltaX); 
            app.setRotation(rectangle, 270 - rotation);

            // enemy move towards player
            rectangle->x += 2 * cos(RAD(rotation)); 
            rectangle->y += 2 * cos(RAD(rotation));
        }
    
        // Player follow mouse
        int deltaY = mouseY - (player->y + player->h / 2);
        int deltaX = mouseX - (player->x + player->w / 2);
        float rotation = xatan(deltaY, deltaX); 
        app.setRotation(player, 270 - rotation);

        switch (pressed_key) {
            case 'w':
                player->x += 10 * cos(RAD(rotation));
                player->y += 10 * sin(RAD(rotation)); 

                break;
        }
        
        if (did_click) {
            auto newProjectile = app.addTexture("bluelaser.png");
            app.setRotation(newProjectile, 270 - rotation);
            newProjectile->x = player->x + player->w / 2;
            newProjectile->y = player->y + player->h / 2;
            activeProjectiles.push_back({newProjectile, true, rotation});
            cout << "sdf" << endl;
        }
        
        int counter = 0;
        bool wasCollided = false;
        for (auto &[projectile, ___useless___, rot] : activeProjectiles) {
            for (auto enemy : enemies) {
                int enemyCounter = 0;
                if (SDL_HasIntersection(projectile, get<0>(enemy)) == SDL_TRUE) { 
                    cout << "Beshan is dog" << endl;
                    get<1>(enemy) -= 10;
                    if (get<1>(enemy) <= 0) {
                        app.removeObject(get<0>(enemy));
                        VECTOR_REMOVE(enemies, enemyCounter);
                    }
                    app.removeObject(projectile);
                    VECTOR_REMOVE(activeProjectiles, counter);
                    wasCollided = true;
                }
                ++enemyCounter;
            }
            if (!wasCollided) {
                float seconds = (clock() - time) / 1000;
                projectile->x += seconds * SPEED * cos(RAD(rot));
                projectile->y += seconds * SPEED * sin(RAD(rot));
                ++counter;
                wasCollided = false;
            }
        }
        time = clock();
        app.render();

    }
    
}
