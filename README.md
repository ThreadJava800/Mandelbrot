# Mandelbrot visualisation
> -Why? It is a very simple task.

> -Yep, but try to optimise it using instruction-level parallelism.

|  |  |
| --- | --- |
| `Compiler` | g++ (GCC) 12.2.1 20230201 files |
| `Optimisation flags` | -msse4.2
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
(words ~ 0.600 ms)