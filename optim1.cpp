#include "mandel.h"

void getMandelbrot(sf::Image* pixelsImage) {
    if (!pixelsImage) return;

    float y0 = factStartY;

    for (int pixelY = 0; pixelY < WINDOW_HEIGHT; pixelY++, y0 += deltaY) {
        float x0 = factStartX;

        for (int pixelX = 0; pixelX < WINDOW_LENGTH; pixelX += 4, x0 += 4 * deltaX) {
            float x0s[4] = {x0, x0 + deltaX, x0 + 2 * deltaX, x0 + 3 * deltaX};
            float y0s[4] = {y0, y0, y0, y0};

            float xs[4]  = {x0s[0], x0s[1], x0s[2], x0s[3]};
            float ys[4]  = {y0s[0], y0s[1], y0s[2], y0s[3]};
            

            short cmpMask = 0;
            int cmp[4] = {0, 0, 0, 0};

            for (int i = 0; i < MAX_REPEAT; i++) {
                float xs2 [4] = {xs[0] * xs[0], xs[1] * xs[1], xs[2] * xs[2], xs[3] * xs[3]};
                float ys2 [4] = {ys[0] * ys[0], ys[1] * ys[1], ys[2] * ys[2], ys[3] * ys[3]};
                float xsys[4] = {xs[0] * ys[0], xs[1] * ys[1], xs[2] * ys[2], xs[3] * ys[3]};

                float radiuses[4] = 
                    {xs2[0] + ys2[0], xs2[1] + ys2[1], xs2[2] + ys2[2], xs2[3] + ys2[3]};

                for (int m = 0; m < 4; m++) {
                    if ((MAX_RADIUS2 < radiuses[m]) && !(cmpMask & (1 << m))) {
                        cmp[m] = i;
                        cmpMask |= (1 << m);
                    }
                }

                xs[0] = xs2[0] - ys2[0] + x0s[0];
                xs[1] = xs2[1] - ys2[1] + x0s[1];
                xs[2] = xs2[2] - ys2[2] + x0s[2];
                xs[3] = xs2[3] - ys2[3] + x0s[3];

                ys[0] = 2 * xsys[0] + y0s[0];
                ys[1] = 2 * xsys[1] + y0s[1];
                ys[2] = 2 * xsys[2] + y0s[2];
                ys[3] = 2 * xsys[3] + y0s[3];
            }

            if (cmpMask) {
                for (int i = 0; i < 4; i++) {
                    if (cmpMask & (1 << i)) {
                        (*pixelsImage).setPixel(pixelX + i, pixelY, sf::Color(cmp[i], cmp[i], cmp[i]));
                    }
                }
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

    sf::Font font;
    font.loadFromFile("font.ttf");

    sf::Text    text;
    text.setFont(font);
    text.setFillColor(sf::Color::White);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || 
                (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
                
                window.close();
            }


            clock_t startTime = clock();
            getMandelbrot(&pixelsImage);

            sprintf(fpsText, "%lf ms", ((double)clock() - (double)startTime) / CLOCKS_PER_SEC * 1000);  // ms
            text.setString(fpsText);

            pixelsTexture.loadFromImage(pixelsImage);
            pixelsSp.     setTexture   (pixelsTexture);

            window.clear();
            window.draw(pixelsSp);
            window.draw(text);
            window.display();
        }
        
    }

    free(pixelArr);
}

int main() {
    runMainCycle();

    return 0;
}