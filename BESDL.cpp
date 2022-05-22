#include "BESDL.hpp"
#include <SDL_ttf.h>
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
SDL_Rect* App::addText (string text, string ttf_path, int fontSize, Uint8 colorR, Uint8 colorG, Uint8 colorB) {
    auto font = TTF_OpenFont(ttf_path.c_str(), fontSize); 
    fonts.push_back(font);


    SDL_Color color = {colorR, colorG, colorB};

    auto surface = TTF_RenderText_Solid(font, text.c_str(), color);
    texture_surfaces.push_back(surface);

    auto texture = SDL_CreateTextureFromSurface(rend, surface);
    textures.push_back(texture);

    texture_rotation.push_back(0.0);
    auto rect = new SDL_Rect[1];
    texture_rectangles.push_back(rect);
    return rect;
}

SDL_Rect* App::addTexture (string path) {
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
    auto dest = new SDL_Rect[1];
    
    // connects our texture with dest to control position
    SDL_QueryTexture(tex, NULL, NULL, &dest->w, &dest->h); // creates a surface to load an image into the main memory

    this->texture_rectangles.push_back(dest);
    this->texture_surfaces.push_back(surface);
    this->textures.push_back(tex);
    texture_rotation.push_back(0.0);
    return dest;
}

void App::removeObject (SDL_Rect *rect) {
    int i = 0;
    for (auto object : this->texture_rectangles) {
        if (rect == object) {
            this->textures.erase(this->textures.begin() + i);
            this->texture_surfaces.erase(this->texture_surfaces.begin() + i); 
            this->texture_rectangles.erase(this->texture_rectangles.begin() + i);
        }
        ++i;
    }
}

void App::setRotation(SDL_Rect *rect, double rotation) {
    int counter = 0;
    for (auto _rect: this->texture_rectangles) {
        if (_rect == rect) {
            this->texture_rotation[counter] = -rotation;
            return;
        }
        ++counter;
        
    }
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

void App::render () {
    SDL_RenderClear(this->rend);
    for (int i = 0; i < this->texture_rectangles.size(); i++) {
        SDL_RenderCopyEx(this->rend, this->textures[i], NULL, this->texture_rectangles[i], texture_rotation[i], NULL, SDL_FLIP_NONE);    
    }
    SDL_RenderPresent(this->rend);
    SDL_Delay(1000 / (float)60);
}

App::~App() {
    for (auto texture : this->textures) {
        SDL_DestroyTexture(texture);
    }
    for (auto surface : this->texture_surfaces) {
        SDL_FreeSurface(surface);
    }
    for (auto font: fonts) {
        TTF_CloseFont(font);
    }
 
    // destroy renderer
    SDL_DestroyRenderer(rend);
 
    // destroy window
    SDL_DestroyWindow(win);
     
    // close SDL
    TTF_Quit();
    SDL_Quit();
}

