# Mandelbrot visualisation
> -Why? It is a very simple task.

> -Yep, but try to optimise it using instruction-level parallelism.

![Mandelbrot visualisation](https://github.com/ThreadJava800/Mandelbrot/blob/master/pic.png)

|  |  |
| --- | --- |
| `Compiler` | g++ (GCC) 12.2.1 |
| `Optimisation flags` | -mavx512f
| `OS` | Arch Linux x86_64 (6.2.7-arch1-1)|
| `CPU` | AMD Ryzen 5 5500U

It's a simple task, where I learned what is instruction-level parallelism. Mostly I used cycle unrolling.\
So.
### Step 1
Just [simple (naive) realization](https://github.com/ThreadJava800/Mandelbrot/blob/master/naive.cpp) (works ~ 1.300 ms)

### Step 2
Added [cycle unrolling](https://github.com/ThreadJava800/Mandelbrot/blob/master/optim1.cpp). But it looks cringe, annoying copy-paste and etc. \
(works ~ 0.600 ms)

### Step 3
Tried to [use cycles](https://github.com/ThreadJava800/Mandelbrot/blob/master/optim2.cpp) and make g++ to unroll it. But... it doesn't work. \
g++ is afraid to do cycle unrolling inside functions\
(works ~ 13.200 ms)

### Step 4
Than I found out that [inline functions](https://github.com/ThreadJava800/Mandelbrot/blob/master/optim3.cpp) are better adapted for msse optimisation. But... didn't work\
(works ~ 13.200 ms)

### Step 5
After it, I found out about intrinsigs! And it [works just fine](https://github.com/ThreadJava800/Mandelbrot/blob/master/optim4.cpp)\
(works ~ 0.600 ms)

### Step 6
Then I switched to __m256 with 8 floats in it.\
[Program](https://github.com/ThreadJava800/Mandelbrot/blob/master/optim5.cpp) started to work twice as fast (~ 0.390 ms)

### Step 7
Then I switched to __m512 with 16 floats in it.\
[Program](https://github.com/ThreadJava800/Mandelbrot/blob/master/optim6.cpp)... didn't work (because my processor does not support avx512 optimisation).

### Comparison table
It's important to mention, that I tested everything on my laptop connected to electricity.

I ran every calculation 100 times and took median time.\
First of all, I compiled everything with -O0 flag:

| Optimisation | Theoretical acceleration | Actual acceleration
| --- | --- | --- |
| `Naive` | 1x (1094.3ms) | 1x (1094.3ms) |
| `SSE4.2` | 4x (273.58ms)  | 1.62x (675.63ms)
| `AVX256` | 8x (136.8ms) | 2.9x (380.32ms) |

Then, with -O1:

| Optimisation | Theoretical acceleration | Actual acceleration
| --- | --- | --- |
| `Naive` | 1x (772.4ms) | 1x (772.4ms) |
| `SSE4.2` | 4x (193.1ms)  | 3.6x (213.93ms)
| `AVX256` | 8x (96.55ms) | 6.4x (121.15ms) |

-O2:

| Optimisation | Theoretical acceleration | Actual acceleration
| --- | --- | --- |
| `Naive` | 1x (437.5ms) | 1x (437.5ms) |
| `SSE4.2` | 4x (109.38ms)  | 3.78x (115.7ms)
| `AVX256` | 8x (54.69ms) | 6.9x (63.16ms) |

-O3:

| Optimisation | Theoretical acceleration | Actual acceleration
| --- | --- | --- |
| `Naive` | 1x (437.5ms) | 1x (437.5ms) |
| `SSE4.2` | 4x (109.38ms)  | 3.79x (115.5ms)
| `AVX256` | 8x (54.69ms) | 7.3x (60ms) |

-Ofast:

| Optimisation | Theoretical acceleration | Actual acceleration
| --- | --- | --- |
| `Naive` | 1x (406.17ms) | 1x (406.17ms) |
| `SSE4.2` | 4x (101.54ms)  | 3.9x (104.24ms)
| `AVX256` | 8x (50.77ms) | 7.2x (56.4ms) |