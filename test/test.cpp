#include <iostream>
#include <math.h>
#define DEG(x) x*(180/M_PI)
#define RAD(x) (x/(float)180)*(float)M_PIde <math.h>
using namespace std;
int main () {
    float y = -1; 
    float x = -1;
    cout << DEG(atan2(y, x)) << endl; // -45

    // y = 1, x = 1, 45 
    // y = 1, x = 0, 
}