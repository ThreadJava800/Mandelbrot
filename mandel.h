#include <SFML/Graphics.hpp>

const int WINDOW_LENGTH = 1080;
const int WINDOW_HEIGHT = 1080;

const int MAX_REPEAT    = 256;
const int MAX_RADIUS2   = 100;

// DEFAULT:
const float factStartX  =  -2;
const float factStartY  =  -1.5;
const float factEndX    =   1;
const float factEndY    =   1.5;

// const float factStartX  =  -5.5/3 * 100000;
// const float factStartY  =  -0.95  * 100000;
// const float factEndX    =   2.5/3 * 100000;
// const float factEndY    =   1.05  * 100000;


const float deltaX      = (factEndX - factStartX) / WINDOW_LENGTH;
const float deltaY      = (factEndY - factStartY) / WINDOW_HEIGHT;

char fpsText[2000] = "";

struct Complex_t {
    int x = 0;
    int y = 0;
};

struct RGB_t {
    short r = 0;
    short g = 0;
    short b = 0;
};


void getMandelbrot(sf::Image* pixelsImage);
void runMainCycle();