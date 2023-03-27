#include "mandel.h"

void getMandelbrot(sf::Image* pixelsImage) {
    if (!pixelsImage) return;

    float x0 = factStartX;

    for (int pixelX = 0; pixelX < WINDOW_LENGTH; pixelX++, x0 += deltaX) {
        float y0 = factStartY;

        for (int pixelY = 0; pixelY < WINDOW_HEIGHT; pixelY++, y0 += deltaY) {
            float x  = x0, y  = y0;         // complex floating point
            
            int i = 0;
            bool needDraw = false;
            for (; i < MAX_REPEAT; i++) {
                float x2 = x * x;
                float y2 = y * y;
                float xy = x * y;

                float radius = x2 + y2;
                if (MAX_RADIUS2 < radius) {
                    needDraw = true;
                    break;
                }

                float updX = x2 - y2 + x0;
                float updY = 2 * xy + y0;

                x = updX;
                y = updY;
            }

            if (needDraw) {
                (*pixelsImage).setPixel(pixelX, pixelY, sf::Color(i, i, i));
            }
        }
    }
}

void runMainCycle() {
    RGB_t* pixelArr = (RGB_t*) calloc(WINDOW_LENGTH * WINDOW_HEIGHT, sizeof(RGB_t));
    if (!pixelArr) return;

    pixelArr[WINDOW_HEIGHT * WINDOW_LENGTH - 1] = {255, 255, 255};

    sf::RenderWindow window(sf::VideoMode(WINDOW_LENGTH, WINDOW_HEIGHT), "Mandelbrot visualization");
    window.setPosition(sf::Vector2i(0, 0));

    // draw pixels
    sf::Image   pixelsImage;
    pixelsImage.create(WINDOW_LENGTH, WINDOW_HEIGHT, sf::Color::Black);

    sf::Texture pixelsTexture;
    sf::Sprite  pixelsSp;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || 
                (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
                
                window.close();
            }

            getMandelbrot(&pixelsImage);

            pixelsTexture.loadFromImage(pixelsImage);
            pixelsSp.     setTexture   (pixelsTexture);

            window.draw(pixelsSp);
            window.display();
        }
        
    }

    free(pixelArr);
}

int main() {
    runMainCycle();

    return 0;
}