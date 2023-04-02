#include <immintrin.h>

#include "mandel.h"


void getMandelbrot(sf::Image* pixelsImage) {
    if (!pixelsImage) return;

    __m512  _2222       = _mm512_set1_ps(2);
    __m512  radius2m512 = _mm512_set1_ps(MAX_RADIUS2);
    __m512i addConst    = _mm512_set1_epi32(0xFFFFFFFF);

    float y0 = factStartY;

    for (int pixelY = 0; pixelY < WINDOW_HEIGHT; pixelY++, y0 += deltaY) {
        float x0 = factStartX;

        for (int pixelX = 0; pixelX < WINDOW_LENGTH; pixelX += 16, x0 += 16 * deltaX) {
            __m512 x0s   = _mm512_set1_ps(x0);
            __m512 _0123 = _mm512_set_ps(15 * deltaX, 14 * deltaX, 13 * deltaX, 12 * deltaX, 
                                         11 * deltaX, 10 * deltaX, 9 * deltaX, 8 * deltaX,
                                         7 * deltaX, 6 * deltaX, 5 * deltaX, 4 * deltaX, 
                                         3 * deltaX, 2 * deltaX, deltaX, 0);
            x0s          = _mm512_add_ps (x0s, _0123);

            __m512 y0s   = _mm512_set1_ps(y0);

            __m512 xs    = x0s; 
            __m512 ys    = _mm512_set1_ps(y0);
            
            // cmp = 4 zeros (int type)                                                                                                                                                                                    
            __m512i cmp = _mm512_setzero_si512();

            for (int i = 0; i < MAX_REPEAT; i++) {
                __m512 xs2  = _mm512_mul_ps(xs, xs);
                __m512 ys2  = _mm512_mul_ps(ys, ys);
                __m512 xsys = _mm512_mul_ps(xs, ys);                                                                                                                          

                __m512 radiuses = _mm512_add_ps(xs2, ys2);

                // generate cmp mask with comparing radiuses with radius2m128
                // 0xFFFFFFFF (success) or 0x0 (failure)
                // check if radiuses <= radius2m128
                __mmask16 cmpMask = _mm512_cmp_ps_mask(radiuses, radius2m512, _CMP_LE_OQ);

                // if met 0xFFFFFFFF -> put 1 else 0 (each byte)
                if (!cmpMask) break;

                cmp = _mm512_sub_epi32(cmp, _mm512_maskz_mov_epi32(cmpMask, addConst));

                xs = _mm512_sub_ps(xs2, ys2);
                xs = _mm512_add_ps(xs, x0s);     // xs[j] = xs2[j] - ys2[j] + x0s[j];

                ys = _mm512_mul_ps(_2222, xsys);
                ys = _mm512_add_ps(ys, y0s);
            }

            int* wCmp = (int*) (&cmp);
            for (int i = 0; i < 16; i++) {
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

    int frames = 0;
    clock_t startTime = clock();

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