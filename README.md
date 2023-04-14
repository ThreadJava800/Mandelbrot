# Mandelbrot visualisation
> -Why? It is a very simple task.

> -Yep, but try to optimise it using instruction-level parallelism.

## Table of Contents
1. [General information](#general)
2. [Progress](#progress)
3. [Comparison table](#compare)
4. [Dive into assembly](#assemble)

## General information <a name="general"></a>

![Mandelbrot visualisation](https://github.com/ThreadJava800/Mandelbrot/blob/master/pic.png)

|  |  |
| --- | --- |
| `Compiler` | g++ (GCC) 12.2.1 |
| `Optimisation flags` | -msse4.2 (-mavx2)
| `OS` | Arch Linux x86_64 (6.2.7-arch1-1)|
| `CPU` | AMD Ryzen 5 5500U

It's a simple task, where I learned what is `instruction-level parallelism`. Mostly I used cycle unrolling.\
So.

## Progress <a name="progress"></a>

### Step 1
Just [simple (naive) realization](https://github.com/ThreadJava800/Mandelbrot/blob/master/naive.cpp)

### Step 2
Added [cycle unrolling](https://github.com/ThreadJava800/Mandelbrot/blob/master/optim1.cpp). But it looks cringe, annoying copy-paste and etc. On the other hand, works faster \

### Step 3
Tried to [use cycles](https://github.com/ThreadJava800/Mandelbrot/blob/master/optim2.cpp) and make g++ to unroll it. But... it doesn't work. \
g++ is afraid to do cycle unrolling inside functions\
(Operating time ~ 13.200 ms)

### Step 4
Than I found out that [inline functions](https://github.com/ThreadJava800/Mandelbrot/blob/master/optim3.cpp) are better adapted for `SSE` optimisation. But... didn't work\
(Operating time ~ 13.200 ms)

### Step 5
After it, I found out about intrinsigs! And it [works just fine](https://github.com/ThreadJava800/Mandelbrot/blob/master/optim4.cpp)\
(Operating time ~ 0.600 ms)

### Step 6
Then I switched to `__m256` with 8 floats in it.\
[Program](https://github.com/ThreadJava800/Mandelbrot/blob/master/optim5.cpp) started to work twice as fast (Operating time ~ 0.390 ms)

### Step 7
Then I switched to `__m512` with 16 floats in it.\
[Program](https://github.com/ThreadJava800/Mandelbrot/blob/master/optim6.cpp)... didn't work (because my processor does not support avx512 optimisation).

## Comparison table <a name="compare"></a>
It's important to mention, that I tested everything on my laptop (plugged in).

I ran every calculation 100 times and took median time.\


First of all, I compiled everything with -O2 flag:


| Optimisation | Theoretical acceleration | Actual acceleration |
| --- | --- | --- |
| `Naive` | 1x (437.5ms) | 1x (437.5 ± 0.65 ms) |
| `SSE4.2` | 4x (109.38ms)  | 3.78x (115.7 ± 0.66 ms) |
| `AVX256` | 8x (54.69ms) | 6.9x (63.16 ± 0.6ms) |

-O3:

| Optimisation | Theoretical acceleration | Actual acceleration |
| --- | --- | --- |
| `Naive` | 1x (437.5ms) | 1x (437.5 ± 0.64 ms) |
| `SSE4.2` | 4x (109.38ms)  | 3.79x (115.5 ± 0.38 ms) |
| `AVX256` | 8x (54.69ms) | 7.3x (60 ± 0.38 ms) | 

-Ofast:

| Optimisation | Theoretical acceleration | Actual acceleration |
| --- | --- | --- |
| `Naive` | 1x (406.17ms) | 1x (406.17 ± 0.51 ms) |
| `SSE4.2` | 4x (101.54ms)  | 3.9x (104.24 ± 0.41 ms) |
| `AVX256` | 8x (50.77ms) | 7.2x (56.4 ± 0.39 ms) |

## Dive into assembly <a name="assemble"></a>