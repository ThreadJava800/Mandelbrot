#include <SFML/Graphics.hpp>

const int WINDOW_LENGTH = 800;
const int WINDOW_HEIGHT = 800;

const int MAX_REPEAT    = 256;
const int MAX_RADIUS2   = 100;

const float factStartX  = -1.5f;
const float factStartY  = -1.5f;
const float factEndX    = 1.5f;
const float factEndY    = 1.5f;

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