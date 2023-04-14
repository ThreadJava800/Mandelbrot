#include "mandel.h"

#include <immintrin.h>
#include <math.h>

void getMandelbrotNaive() {
    float y0 = factStartY;
    int *arr = (int*) calloc(WINDOW_HEIGHT * WINDOW_LENGTH, sizeof(int));

    for (int pixelY = 0; pixelY < WINDOW_HEIGHT; pixelY++, y0 += deltaY) {
        float x0 = factStartX;

        for (int pixelX = 0; pixelX < WINDOW_LENGTH; pixelX++, x0 += deltaX) {
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

                x = x2 - y2 + x0;
                y = 2 * xy + y0;
            }

            if (needDraw) {
                arr[pixelY * WINDOW_HEIGHT + pixelX] = i;
            }
        }
    }

    free(arr);
}

// void getMandelbrotSSE() {
//     __m128 _2222       = _mm_set_ps1(2);
//     __m128 radius2m128 = _mm_set_ps1(MAX_RADIUS2);

//     float y0 = factStartY;
//     int *arr = (int*) calloc(WINDOW_HEIGHT * WINDOW_LENGTH, sizeof(int));

//     for (int pixelY = 0; pixelY < WINDOW_HEIGHT; pixelY++, y0 += deltaY) {
//         float x0 = factStartX;

//         for (int pixelX = 0; pixelX < WINDOW_LENGTH; pixelX += 4, x0 += 4 * deltaX) {
//             __m128 x0s   = _mm_set_ps1(x0);
//             __m128 _0123 = _mm_set_ps (3 * deltaX, 2 * deltaX, deltaX, 0);
//             x0s          = _mm_add_ps (x0s, _0123);

//             __m128 y0s   = _mm_set_ps1(y0);

//             __m128 xs    = x0s; 
//             __m128 ys    = _mm_set_ps1(y0);
            
//             // cmp = 4 zeros (int type)                                                                                                                                                                                    
//             __m128i cmp = _mm_setzero_si128();

//             for (int i = 0; i < MAX_REPEAT; i++) {
//                 __m128 xs2  = _mm_mul_ps(xs, xs);
//                 __m128 ys2  = _mm_mul_ps(ys, ys);
//                 __m128 xsys = _mm_mul_ps(xs, ys);                                                                                                                          

//                 __m128 radiuses = _mm_add_ps(xs2, ys2);

//                 // generate cmp mask with comparing radiuses with radius2m128
//                 // 0xFFFFFFFF (success) or 0x0 (failure)
//                 // check if radiuses <= radius2m128
//                 __m128 cmpMask = _mm_cmple_ps(radiuses, radius2m128);

//                 // if met 0xFFFFFFFF -> put 1 else 0 (each byte)
//                 if (!_mm_movemask_ps(cmpMask)) break;

//                 cmp = _mm_sub_epi32(cmp, _mm_castps_si128(cmpMask));

//                 xs = _mm_sub_ps(xs2, ys2);
//                 xs = _mm_add_ps(xs, x0s);     // xs[j] = xs2[j] - ys2[j] + x0s[j];

//                 ys = _mm_mul_ps(_2222, xsys);
//                 ys = _mm_add_ps(ys, y0s);
//             }

//             int* wCmp = (int*) (&cmp);
//             for (int i = 0; i < 4; i++) {
//                 arr[pixelY * WINDOW_HEIGHT + pixelX + i] = wCmp[i];
//             }
//         }
//     }

//     free(arr);
// }

// void getMandelbrotAVX() {
//     __m256 _2222       = _mm256_set1_ps(2);
//     __m256 radius2m256 = _mm256_set1_ps(MAX_RADIUS2);

//     float y0 = factStartY;
//     int *arr = (int*) calloc(WINDOW_HEIGHT * WINDOW_LENGTH, sizeof(int));

//     for (int pixelY = 0; pixelY < WINDOW_HEIGHT; pixelY++, y0 += deltaY) {
//         float x0 = factStartX;

//         for (int pixelX = 0; pixelX < WINDOW_LENGTH; pixelX += 8, x0 += 8 * deltaX) {
//             __m256 x0s   = _mm256_set1_ps(x0);
//             __m256 _0123 = _mm256_set_ps(7 * deltaX, 6 * deltaX, 5 * deltaX, 4 * deltaX, 
//                                          3 * deltaX, 2 * deltaX, deltaX, 0);
//             x0s          = _mm256_add_ps (x0s, _0123);

//             __m256 y0s   = _mm256_set1_ps(y0);

//             __m256 xs    = x0s; 
//             __m256 ys    = _mm256_set1_ps(y0);
            
//             // cmp = 4 zeros (int type)                                                                                                                                                                                    
//             __m256i cmp = _mm256_setzero_si256();

//             for (int i = 0; i < MAX_REPEAT; i++) {
//                 __m256 xs2  = _mm256_mul_ps(xs, xs);
//                 __m256 ys2  = _mm256_mul_ps(ys, ys);
//                 __m256 xsys = _mm256_mul_ps(xs, ys);                                                                                                                          

//                 __m256 radiuses = _mm256_add_ps(xs2, ys2);

//                 // generate cmp mask with comparing radiuses with radius2m128
//                 // 0xFFFFFFFF (success) or 0x0 (failure)
//                 // check if radiuses <= radius2m128
//                 __m256 cmpMask = _mm256_cmp_ps(radiuses, radius2m256, _CMP_LE_OQ);

//                 // if met 0xFFFFFFFF -> put 1 else 0 (each byte)
//                 if (!_mm256_movemask_ps(cmpMask)) break;

//                 cmp = _mm256_sub_epi32(cmp, _mm256_castps_si256(cmpMask));

//                 xs = _mm256_sub_ps(xs2, ys2);
//                 xs = _mm256_add_ps(xs, x0s);     // xs[j] = xs2[j] - ys2[j] + x0s[j];

//                 ys = _mm256_mul_ps(_2222, xsys);
//                 ys = _mm256_add_ps(ys, y0s);
//             }

//             int* wCmp = (int*) (&cmp);
//             for (int i = 0; i < 8; i++) {
//                 arr[pixelY * WINDOW_HEIGHT + pixelX + i] = wCmp[i];
//             }
//         }
//     }

//     free(arr);
// }

int main() {
    double *times = (double*) calloc(100, sizeof(double));

    clock_t startTime = {};
    double allTime = 0;
    for (int i = 0; i < 100; i++) {
        startTime = clock();
        getMandelbrotNaive();
        int time = ((double)clock() - (double)startTime) / CLOCKS_PER_SEC * 1000;
        allTime += time;

        times[i] = time;
    }

    double median = allTime / 100;
    double sum = 0;
    for (int i = 0; i < 100; i++) {
        sum += (times[i] - median) * (times[i] - median);
    }

    sum /= 100;

    printf("%lf %lf\n", sqrt(sum), median);

    free(times);

    return 0;
}