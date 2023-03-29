#include "mandel.h"

void getMandelbrot(sf::Image* pixelsImage) {
    if (!pixelsImage) return;

    float y0 = factStartY;

    for (int pixelY = 0; pixelY < WINDOW_HEIGHT; pixelY++, y0 += deltaY) {
        float x0 = factStartX;

        for (int pixelX = 0; pixelX < WINDOW_LENGTH; pixelX += 4, x0 += 4 * deltaX) {
            float x0s[4] = {}; for (int i = 0; i < 4; i++) x0s[i] = x0 + i * deltaX; 
            float y0s[4] = {}; for (int i = 0; i < 4; i++) y0s[i] = y0;

            float xs[4]  = {}; for (int i = 0; i < 4; i++) xs[i] = x0s[i]; 
            float ys[4]  = {}; for (int i = 0; i < 4; i++) ys[i] = y0;
            

            short cmpMask = 0;                                                                                                                                                                                                  
            int cmp[4] = {0, 0, 0, 0};

            for (int i = 0; i < MAX_REPEAT; i++) {
                float xs2 [4] = {}; for (int j = 0; j < 4; j++) xs2[j]  = xs[j] * xs[j];
                float ys2 [4] = {}; for (int j = 0; j < 4; j++) ys2[j]  = ys[j] * ys[j];
                float xsys[4] = {}; for (int j = 0; j < 4; j++) xsys[j] = xs[j] * ys[j];                                                                                                                            

                float radiuses[4] = {}; for (int j = 0; j < 4; j++) radiuses[j] = xs2[j] + ys2[j]; 

                int checkMask = 0;
                int mask[4] = {};   for (int j = 0; j < 4; j++) mask[j] = (MAX_RADIUS2 >= radiuses[j]) ? 0xFFFFFFFF : 0;
                for (int j = 0; j < 4; j++) checkMask |= (mask[j] == 0xFFFFFFFF ? 1 : 0) << j;

                if (!checkMask) break;

                for (int j = 0; j < 4; j++) cmp[j] = cmp[j] - mask[j];

                for (int j = 0; j < 4; j++) xs[j] = xs2[j] - ys2[j] + x0s[j];
                for (int j = 0; j < 4; j++) ys[j] = 2 * xsys[j] + y0s[j];
            }

                for (int i = 0; i < 4; i++) {
                    (*pixelsImage).setPixel(pixelX + i, pixelY, sf::Color(cmp[i], cmp[i], cmp[i]));
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