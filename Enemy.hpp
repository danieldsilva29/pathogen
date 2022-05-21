#pragma once

#include <map>
#include <iostream>
#include <cstring>
#include <string>
#include <stdlib.h>
#include <vector>
#include <math.h>
#include <SDL2/SDL.h>

#include "BESDL.hpp"
using namespace std;

class Entities {
    private: 
        vector<float> directions;
        string image_file;
        int max_num;
        bool is_enemy;
        int movement_speed; 
        float duplication_prob;
    public: 
        vector<SDL_Rect*> rectangles;
        Entities (App* app, int movement_speed, int initial_amount, int max_num, string image_file, bool is_enemy, float duplication_prob);
        bool render (App* app, int playerX=-1, int playerY=-1); 
};