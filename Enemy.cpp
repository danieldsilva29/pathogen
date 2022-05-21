#include "Enemy.hpp"

Entities :: Entities (App* app, int movement_speed, int initial_amount, int max_num, string image_file, bool is_enemy, float duplication_prob) {
    // randomly initialize multiple entities
    this->duplication_prob = duplication_prob;
    this->is_enemy = is_enemy;
    this->image_file = image_file;
    this->movement_speed = movement_speed;
    for (int i = 0; i < initial_amount; i++) {
        SDL_Rect* rect = app->addTexture(image_file);

        float range = rand() % 2000 / (float) 100 + 200; 
        float direction = rand() % 360; 
        float radians = direction * (float) M_PI / (float) 180;
        float x = range * cos(radians);
        float y = range * sin(radians);

        rect->x = x; 
        rect->y = y; 
        if (this->is_enemy) {
            rect->w = rect->h = 100;
        }
        else {
            rect->w = 50; 
            rect->h = 100;
        }

        this->directions.push_back(direction);
        this->rectangles.push_back(rect);
    }
}

bool Entities :: render (App* app, int playerX, int playerY) {
    // // grow if enemy!
    // if (this->is_enemy) {
    //     int randomNum = rand() % 100;
    //     if (randomNum <= this->duplication_prob * 100) {
    //         SDL_Rect* rect = app->addTexture(this->image_file);
    //         float range = rand() % 2000 / (float) 100 + 200; 
    //         float direction = rand() % 360; 
    //         float radians = direction * (float) M_PI / (float) 180;
    //         float x = range * cos(radians);
    //         float y = range * sin(radians);

    //         rect->x = x; 
    //         rect->y = y; 

    //         this->directions.push_back(direction);
    //         this->rectangles.push_back(rect); 
    //     }
    // }

    // render all entities
    for (int i = 0; i < this->rectangles.size(); i++) {
        SDL_Rect* rect = this->rectangles[i];
        int direction = this->directions[i];
        // if enemy, move towards player
        if (this->is_enemy) {
            float distX = playerX - rect->x; 
            float distY = playerY - rect->y; 
            float angle = atan2(distY, distX);
            app->setRotation(rect, angle);  
            rect->x -= this->movement_speed * cos(angle); 
            rect->y -= this->movement_speed * sin(angle);
            this->directions[i] = angle;
        }
        // if player, move using WASD keys
        if (!this->is_enemy) {
            auto [key, mouseX, mouseY, didClick, shouldQuit] = app->getInteraction();
            
            if (key == 'w') {
                float radians = direction * (float) M_PI / (float) 180;
                rect->y -= this->movement_speed * sin(radians);
                rect->x -= this->movement_speed * cos(radians);
            }
            if (key == 'a') {
                direction += 10;
            }
            if (key == 's') {
                float radians = direction * (float) M_PI / (float) 180;
                rect->y += this->movement_speed * sin(radians);
                rect->x += this->movement_speed * cos(radians);
            }
            if (key == 'd') {
                direction -= 10;
            }
            if (shouldQuit) {
                return false;
            }
            
            app->setRotation(rect, direction); 
            this->directions[i] = direction;
        }

        // render entity
        app->render();
    } 
    return true;
}

