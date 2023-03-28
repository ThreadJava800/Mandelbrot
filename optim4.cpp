#include <emmintrin.h>

#include "mandel.h"


void getMandelbrot(sf::Image* pixelsImage) {
    if (!pixelsImage) return;

    __m128 _2222       = _mm_set_ps1(2);
    __m128 radius2m128 = _mm_set_ps1(MAX_RADIUS2);

    float y0 = factStartY;

    for (int pixelY = 0; pixelY < WINDOW_HEIGHT; pixelY++, y0 += deltaY) {
        float x0 = factStartX;

        for (int pixelX = 0; pixelX < WINDOW_LENGTH; pixelX += 4, x0 += 4 * deltaX) {
            __m128 x0s   = _mm_set_ps1(x0);
            __m128 _0123 = _mm_set_ps (0, deltaX, 2 * deltaX, 3 * deltaX);
            x0s          = _mm_add_ps (x0s, _0123);

            __m128 y0s   = _mm_set_ps1(y0);

            __m128 xs    = x0s; 
            __m128 ys    = _mm_set_ps1(y0);
            
            // cmp = 4 zeros (int type)                                                                                                                                                                                    
            __m128i cmp = _mm_setzero_si128();

            for (int i = 0; i < MAX_REPEAT; i++) {
                __m128 xs2  = _mm_mul_ps(xs, xs);
                __m128 ys2  = _mm_mul_ps(ys, ys);
                __m128 xsys = _mm_mul_ps(xs, ys);                                                                                                                          

                __m128 radiuses = _mm_add_ps(xs2, ys2);

                // generate cmp mask with comparing radiuses with radius2m128
                // 0xFFFFFFFF (success) or 0x0 (failure)
                // check if radiuses <= radius2m128
                __m128 cmpMask = _mm_cmple_ps(radiuses, radius2m128);

                // if met 0xFFFFFFFF -> put 1 else 0 (each byte)
                if (!_mm_movemask_ps(cmpMask)) break;

                cmp = _mm_sub_epi32(cmp, _mm_castps_si128(cmpMask));

                xs = _mm_sub_ps(xs2, ys2);
                xs = _mm_add_ps(xs, x0s);     // xs[j] = xs2[j] - ys2[j] + x0s[j];

                ys = _mm_mul_ps(_2222, xsys);
                ys = _mm_add_ps(ys, y0s);
            }

            int* wCmp = (int*) (&cmp);
            for (int i = 0; i < 4; i++) {
                pixelsImage->setPixel(pixelX + i, pixelY, sf::Color(wCmp[i], wCmp[i], wCmp[i]));
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