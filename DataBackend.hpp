#include "Vertex.hpp"
#include <vector>
#include <string>
#include <iostream>
#include "Enemy.hpp"
using namespace std;

class SpaceShip {
    public:
        Entities owner;
        int roundsLeft;
        void render();
        void shootStuff();
};

class SpaceshipProjectile {
    public:
        float xcoords, ycoords;
        float direction; //Measured as an angle
        float speed;
};

class Planet {
    public:
        int r, g, b;
        Entities *colonizer;
};




