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

    texture->setCoords(800, 0, 200, 800);
    texture2->setCoords(200, 0, 200, 800);
    text->setCoords(200, 500, 800, 200);

    int rot = 0;
    int close = 0;
    while (!close) {
 
        // Events management
        auto [pressed_key, mouseX, mouseY, did_click, should_close] = app.getInteraction();
        if (should_close) {
            close = true;
        }
        if (pressed_key == 'w') {
            text->getCoords(COORD_Y) -= 10; 
        }
        if (pressed_key == 'd') {
            text->getCoords(COORD_X) -= 10;
        }

        // right boundary
        if (text->getCoords(COORD_X) + text->getCoords(COORD_WIDTH) > 1000)
            text->getCoords(COORD_X) = 1000 - text->getCoords(COORD_WIDTH);
        // left boundary
        if (text->getCoords(COORD_X) < 0)
            text->getCoords(COORD_X) = 0;
 
        // bottom boundary
        if (text->getCoords(COORD_Y) + text->getCoords(COORD_HEIGHT) > 1000)
            text->getCoords(COORD_Y) = 1000 - text->getCoords(COORD_HEIGHT);
 
        // upper boundary
        if (text->getCoords(COORD_Y) < 0)
            text->getCoords(COORD_Y) = 0;
        
        app.setRotation(text, rot);
        // Render
        app.render();
        rot += 10;
    }
}