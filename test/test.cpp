#include <iostream>
#include <math.h>
#define DEG(x) x*(180/M_PI)
#define RAD(x) (x/(float)180)*(float)M_PI 
using namespace std;
int main () {
    float degree = RAD(250);
    cout << cos(degree) << endl; // -45
    cout << sin(degree) << endl; // -45

    // y = 1, x = 1, 45 
    // y = 1, x = 0, 
}