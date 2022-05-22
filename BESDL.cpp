#include "BESDL.hpp"
#include "Utils.hpp"
#include <SDL2/SDL_ttf.h>
#include <map>

App::App (string name, int width, int height) {
    this->window_width = width;
    this->window_height = height;
    // returns zero on success else non-zero
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
    }
    TTF_Init();
    this->win = SDL_CreateWindow(name.c_str(), // creates a window
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       width, height, 0);
 
    // triggers the program that controls
    // your graphics hardware and sets flags
    Uint32 render_flags = SDL_RENDERER_ACCELERATED;
 
    // creates a renderer to render our images
    this->rend = SDL_CreateRenderer(win, -1, render_flags);
}

// https://stackoverflow.com/questions/22886500/how-to-render-text-in-sdl2
shared_ptr<ShapeObject> App::addText (string text, string ttf_path, int fontSize, Uint8 colorR, Uint8 colorG, Uint8 colorB) {
    auto ret =  make_shared<ShapeObject>(new ShapeObject(this->rend, text, ttf_path, fontSize, colorR, colorG, colorB));
    this->objects.push_back(ret);
    return ret;
}

shared_ptr<ShapeObject> App::addTexture(string path) {
    // creates a surface to load an image into the main memory
    auto ret = make_shared<ShapeObject>(new ShapeObject(this->rend, path));
    this->objects.push_back(ret);
    return ret;
}


ShapeObject::ShapeObject (ShapeObject* obj) {
    
}


tuple<char, int, int, bool, bool> App::getInteraction () {
    char pressed_key;
    int mouseX = 0; 
    int mouseY = 0;
    bool did_click = false; 
    bool should_close = false;
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                should_close = 1;
                break;
            case SDL_TEXTINPUT:
                pressed_key = event.text.text[0];
                break;
            case SDL_MOUSEBUTTONDOWN: 
                did_click = true;
                break;
        }
    }
    SDL_GetMouseState(&mouseX, &mouseY);
    return {pressed_key, mouseX, mouseY, did_click, should_close};
}

void App::render() {
    SDL_RenderClear(this->rend);
    int counter = 0;
    for (auto object : objects) {
        if (auto tmp = object.lock()) {
            SDL_RenderCopyEx(rend, tmp->texture, NULL, tmp->texture_rectangle, tmp->rotation, NULL, SDL_FLIP_NONE);
            ++counter;
        } else {
            VECTOR_REMOVE(objects, counter);
        };
    }
    SDL_RenderPresent(this->rend);
    SDL_Delay(1000 / (float)60);
}

App::~App() {
 
    // destroy renderer
    SDL_DestroyRenderer(rend);
 
    // destroy window
    SDL_DestroyWindow(win);
     
    // close SDL
    TTF_Quit();
    SDL_Quit();
}

ShapeObject::ShapeObject(SDL_Renderer *rend, string path) {
    // creates a surface to load an image into the main memory
    SDL_Surface* surface;
 
    // please provide a path for your image
    surface = IMG_Load(path.c_str());
 
    // loads image to our graphics hardware memory.
    SDL_Texture* tex = SDL_CreateTextureFromSurface(this->rend, surface);
    // clears main-memory
    SDL_FreeSurface(surface);
 
    // let us control our image position
    // so that we can move it with our keyboard.
    this->texture_rectangle = new SDL_Rect[1];
    
    // connects our texture with dest to control position
    SDL_QueryTexture(tex, NULL, NULL, &this->texture_rectangle->w, &this->texture_rectangle->h); // creates a surface to load an image into the main memory

    texture_surface = surface;
    texture = tex;
    rotation = 0.0;
    this->rend = rend;
}

ShapeObject::ShapeObject(SDL_Renderer *rend, string text, string ttf_path, int fontSize, Uint8 colorR, Uint8 colorG, Uint8 colorB) {
    auto font = TTF_OpenFont(ttf_path.c_str(), fontSize); 
    this->font = font;


    SDL_Color color = {colorR, colorG, colorB};

    auto surface = TTF_RenderText_Solid(font, text.c_str(), color);
    texture_surface = surface;

    this->texture = SDL_CreateTextureFromSurface(rend, surface);

    rotation = 0.0;
    texture_rectangle = new SDL_Rect[1];
}

ShapeObject::~ShapeObject() {
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(texture_surface);
    if (font != NULL) TTF_CloseFont(font);
}

tuple<int&, int&, int&, int&> ShapeObject::getCoords() {
    return {
        getCoords(COORD_X),
        getCoords(COORD_Y),
        getCoords(COORD_WIDTH),
        getCoords(COORD_HEIGHT)
    };
}

int& ShapeObject::getCoords(CoordField i) {
    switch(i) {
        case COORD_X:
            return this->texture_rectangle->x;
        case COORD_Y:
            return this->texture_rectangle->y;
        case COORD_WIDTH:
            return this->texture_rectangle->w;
        case COORD_HEIGHT:
            return this->texture_rectangle->h;
    }
}

bool ShapeObject::hasIntersection(shared_ptr<ShapeObject> with) {
    return SDL_HasIntersection(this->texture_rectangle, with->texture_rectangle) == SDL_TRUE ? true: false;
}

void ShapeObject::setCoords(int x, int y, int w, int h) {
    if (x != -1) this->texture_rectangle->x = x;
    if (y != -1) this->texture_rectangle->y = y;
    if (w != -1) this->texture_rectangle->w = w;
    if (h != -1) this->texture_rectangle->h = h; 
}