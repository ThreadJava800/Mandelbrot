#include "mandel.h"

//==============INLINE FUNCTIONS==================
// set arr with given value 4 times
inline void mm_set_ps1(float* arr, const float value) { for (int i = 0; i < 4; i++) arr[i] = value; }
// copy arr of 4 to another
inline void mm_set_ps (float* arr1, float* arr2) { for (int i = 0; i < 4; i++) arr1[i] = arr2[i];  }
// add one arr of 4 to another
inline void mm_add_ps (float* dest, float* arr1, float* arr2) { for (int i = 0; i < 4; i++) dest[i] = arr1[i] + arr2[i]; }
// sub one arr of 4 to another
inline void mm_sub_ps (float* dest, float* arr1, float* arr2) { for (int i = 0; i < 4; i++) dest[i] = arr1[i] - arr2[i]; }
// mul one arr of 4 to another
inline void mm_mul_ps (float* dest, float* arr1, float* arr2) { for (int i = 0; i < 4; i++) dest[i] = arr1[i] * arr2[i]; }

// arr for inline func
float _0123[4] = {0, deltaX, 2 * deltaX, 3 * deltaX};
float _2222[4] = {2, 2, 2, 2};

void getMandelbrot(sf::Image* pixelsImage) {
    if (!pixelsImage) return;

    float y0 = factStartY;

    for (int pixelY = 0; pixelY < WINDOW_HEIGHT; pixelY++, y0 += deltaY) {
        float x0 = factStartX;

        for (int pixelX = 0; pixelX < WINDOW_LENGTH; pixelX += 4, x0 += 4 * deltaX) {
            float x0s[4] = {}; mm_set_ps1(x0s, x0);
                               mm_add_ps (x0s, x0s, _0123);

            float y0s[4] = {}; mm_set_ps1(y0s, y0);

            float xs[4]  = {}; mm_set_ps (xs, x0s); 
            float ys[4]  = {}; mm_set_ps1(ys, y0);
            

            short cmpMask = 0;                                                                                                                                                                                                  
            int cmp[4] = {0, 0, 0, 0};

            for (int i = 0; i < MAX_REPEAT; i++) {
                float xs2 [4] = {}; mm_mul_ps(xs2, xs, xs);
                float ys2 [4] = {}; mm_mul_ps(ys2, ys, ys);
                float xsys[4] = {}; mm_mul_ps(xsys, xs, ys);                                                                                                                          

                float radiuses[4] = {}; mm_add_ps(radiuses, xs2, ys2);

                for (int m = 0; m < 4; m++) {
                    if ((MAX_RADIUS2 < radiuses[m]) && !(cmpMask & (1 << m))) {
                        cmp[m] = i;
                        cmpMask |= (1 << m);
                    }
                }

                mm_sub_ps(xs, xs2, ys2);
                mm_add_ps(xs, xs, x0s);     // xs[j] = xs2[j] - ys2[j] + x0s[j];

                mm_mul_ps(ys, _2222, xsys);
                mm_add_ps(ys, ys, y0s);
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