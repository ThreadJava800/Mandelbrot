#include <SFML/Graphics.hpp>

const int WINDOW_LENGTH = 1920;
const int WINDOW_HEIGHT = 1080;

const int MAX_REPEAT    = 256;
const int MAX_RADIUS2   = 100;

const float factStartX  = -20/9;
const float factStartY  = -0.95f;
const float factEndX    = 12/9;
const float factEndY    = 1.05f;

const float deltaX      = (factEndX - factStartX) / WINDOW_LENGTH;
const float deltaY      = (factEndY - factStartY) / WINDOW_HEIGHT;

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