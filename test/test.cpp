#include <iostream>
#include <math.h>
#define DEG(x) x*(180/M_PI)
#define RAD(x) (x/(float)180)*(float)M_PIde <math.h>
using namespace std;
int main () {
    float degree = RAD(30);
    cout << DEG(sin(degree)) << endl; // -45
    cout << DEG(cos(degree)) << endl; // -45

    // y = 1, x = 1, 45 
    // y = 1, x = 0, 
}