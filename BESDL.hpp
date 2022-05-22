#pragma once
extern "C" {
    #include <SDL2/SDL.h>
    #include <SDL2/SDL_image.h>
    #include <SDL2/SDL_timer.h>
    #include <SDL2/SDL_ttf.h>
}
#include <string.h>
#include <tuple>
#include <iostream>
#include <vector>
#include <map> 
#include <memory>

using namespace std;
#define GET(obj, x) get<x>(obj)

typedef enum {
    COORD_X,
    COORD_Y,
    COORD_WIDTH,
    COORD_HEIGHT
} CoordField;

class ShapeObject {
    //Submit an earlier version
    //Wait shoot do we even have the working version
    //You go check that, I'll try to continue fixing this

    
    private:
        SDL_Renderer *rend;
        SDL_Texture *texture;
        SDL_Rect *texture_rectangle = new SDL_Rect[1]; 
        SDL_Surface *texture_surface = new SDL_Surface[1];
        TTF_Font *font;
        friend class App;
    public:
        void setCoords(int x = -1, int y = -1, int w = -1, int h = -1);
        tuple<int&, int&, int&, int&> getCoords();
        int& getCoords(CoordField i);
        bool hasIntersection(shared_ptr<ShapeObject> with);
        double rotation;
        ShapeObject(ShapeObject* obj);
        ShapeObject(SDL_Renderer *rend, string path);
        ShapeObject(SDL_Renderer *rend, string text, string ttf_path, int fontSize = 14, Uint8 colorR=255, Uint8 colorG=255, Uint8 colorB=255);
        ~ShapeObject();
        void nothing() {
            cout << "I'm here at: " << this << endl;
        }
        void print();
};
class App {
    private: 
        SDL_Window* win;
        SDL_Renderer* rend;
        vector<weak_ptr<ShapeObject>> objects;
    public: 
        int window_width;
        int window_height;
        App (string name = "GAME", int width=1000, int height=1000); 
        shared_ptr<ShapeObject> addTexture (string path); 
        shared_ptr<ShapeObject> addText (string text, string ttf_path, int fontSize=14, Uint8 colorR=255, Uint8 colorG=255, Uint8 colorB=255); 
        tuple<char, int, int, bool, bool> getInteraction(); 
        void render();
        void draw();
        ~App();
};
