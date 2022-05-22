#include "BESDL.hpp"
#include <string.h>
#include <tuple>
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
    App app = App("YOU LITTLE PERSON", 1000, 1000);
    
    auto texture = app.addTexture("AnotherImage.jpeg");
    auto texture2 = app.addTexture("AnotherImage.jpeg");
    auto text = app.addText("HI BESHAN", "./font.ttf", 50, 255, 255, 255); 

    // adjust height and width of our image box.

    texture->x = 800;
    texture->y = 0;
    texture->w = 200;
    texture->h = 800;

    texture2->x = 200;
    texture2->y = 0;
    texture2->w = 200;
    texture2->h = 800;
    
    text->x = 200;
    text->y = 500;
    text->w = 800;
    text->h = 200;

    int rot = 0;
    int close = 0;
    while (!close) {
 
        // Events management
        auto [pressed_key, mouseX, mouseY, did_click, should_close] = app.getInteraction();
        if (should_close) {
            close = true;
        }
        if (pressed_key == 'w') {
            text->y -= 10; 
        }
        if (pressed_key == 'd') {
            text->x -= 10;
        }

        // right boundary
        if (text->x + text->w > 1000)
            text->x = 1000 - text->w;
        // left boundary
        if (text->x < 0)
            text->x = 0;
 
        // bottom boundary
        if (text->y + text->h > 1000)
            text->y = 1000 - text->h;
 
        // upper boundary
        if (text->y < 0)
            text->y = 0;
        
        app.setRotation(text, rot);
        // Render
        app.render();
        rot += 10;
    }
}