#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>
#include <tuple>
#include <iostream>
#include <vector>
#include <map> 
using namespace std;

class App {
    private: 
        SDL_Window* win;
        SDL_Renderer* rend;
        vector<SDL_Texture*> textures;
        vector<SDL_Rect*> texture_rectangles;
        vector<SDL_Surface*> texture_surfaces;
        vector<TTF_Font*> fonts;
        vector<double> texture_rotation;
        map<SDL_Rect*, tuple<int, int, int, int, float, int>> movementMap;
        void translateMotion();
    public: 
        int window_width;
        int window_height;
        App (string name = "GAME", int width=1000, int height=1000); 
        SDL_Rect* addTexture (string path); 
        SDL_Rect* addText (string text, string ttf_path, int fontSize=14, Uint8 colorR=255, Uint8 colorG=255, Uint8 colorB=255); 
        tuple<char, int, int, bool, bool> getInteraction(); 
        void render();
        void draw();
        void setRotation(SDL_Rect *rect, double rotation); //THIS ROTATES CLOCKWISE
        ~App();
};
#include "Vertex.hpp"
