# Визуализация множества Мандельброта
> Визуализация множества Мандельброта с SIMD оптимизациями.

## Содержание
1. [Общая информация](#general)
2. [Ход работы](#progress)
3. [Сравнение реализаций](#compare)
4. [Углубимся в ассемблер](#assemble)
5. [Заключение](#conclusion)

## Общая информация <a name="general"></a>

![Mandelbrot visualisation](https://github.com/ThreadJava800/Mandelbrot/blob/master/testpics/pic.png)

|  |  |
| --- | --- |
| `Компилятор` | g++ (GCC) 12.2.1 |
| `Флаги оптимазации` | -msse4.2 (-mavx2)
| `ОС` | Arch Linux x86_64 (6.2.7-arch1-1)|
| `Процессор` | AMD Ryzen 5 5500U

Я исследовал влияние <em>intrinsic</em> функций на быстродействие этого несложного алгоритма.\
Также, моей целью было ближе познакомиться с понятием <em>параллелизма уровня инструкций</em>.

## Ход работы <a name="progress"></a>

### Шаг 1
[Простая (наивная) реализация](https://github.com/ThreadJava800/Mandelbrot/blob/master/naive.cpp).\
Был написан алгоритм, который для каждой точки $(x_0, y_0)$ на плоскости рассчитывает следующую последовательность:\
$x_{i+1} = {x_{i}}^{2} - {y_{i}}^{2} + x_0$\
$y_{i+1} = 2 * x_i * y_i + y_0$\
Зададим максимальный радиус, и будем считать, что точка последовательности лежит внутри множества, если её расстояние от начала координат ≤ максимального радиуса.\
Цвет точки $(x_0, y_0)$ определяется номером итерации, на которой точка последовательности вышла из множества.\
Ограничим количество итераций константой, и в случае, если точка последовательности не выйдет из множества за меньшее количество итераций, окрасим точку в цвет, зависящий от этой константы.

Визуализация реализована при помощи библиотеки [SFML](https://www.sfml-dev.org/).

![naive](https://github.com/ThreadJava800/Mandelbrot/blob/master/testpics/naive.png)

### Шаг 2 <a name="step2"></a>
В ручную добавил [развёртку циклов](https://github.com/ThreadJava800/Mandelbrot/blob/master/optim1.cpp).\
Код стал работать в 2 раза быстрее, однако стал менее читаемым:\
![optim1_start](https://github.com/ThreadJava800/Mandelbrot/blob/master/testpics/optim1_start.png)\
`30 строк пропущено`\
![optim1_end](https://github.com/ThreadJava800/Mandelbrot/blob/master/testpics/optim1_end.png)


### Шаг 3
Решил сократить количество кода с помощью [циклов](https://github.com/ThreadJava800/Mandelbrot/blob/master/optim2.cpp), в надежде, что `g++` сумеет развернуть их. Но, он не смог :). Например,\
![optim2_1](https://github.com/ThreadJava800/Mandelbrot/blob/master/testpics/optim2_1.png)\
было заменено на\
![optim2_2](https://github.com/ThreadJava800/Mandelbrot/blob/master/testpics/optim2_2.png).\
Однако, `g++` без флагов оптимизации не рискует разворачивать циклы\
В результате, программа стала работать даже дольше, чем наивная реализация (примерно в 20 раз).

### Шаг 4
Затем я решил заменить циклы на [inline функции](https://github.com/ThreadJava800/Mandelbrot/blob/master/optim3.cpp), ведь их компилятор с большей вероятностью свернёт.\
Таким образом,\
![optim2_2](https://github.com/ThreadJava800/Mandelbrot/blob/master/testpics/optim2_2.png)\
был заменён на\
![optim3_1](https://github.com/ThreadJava800/Mandelbrot/blob/master/testpics/optim3_1.png),\
где\
![optim3_2](https://github.com/ThreadJava800/Mandelbrot/blob/master/testpics/optim3_2.png)\
*`_2222` - float массив из 4-х двоек*\
Однако, на скорость работы данные манипуляции не повлияли.\
Эти <em>inline</em> функции неслучайно были названы аналогично некоторым <em>intrinsic</em> функциям, ведь этот шаг был подготовительным к следующему.

### Шаг 5
Далее, я решил перейти на [intrinsic функции](https://github.com/ThreadJava800/Mandelbrot/blob/master/optim4.cpp)\
В частности использовались `SSE` оптимизации.\
Так, например, уже известные нам строки\
![optim3_1](https://github.com/ThreadJava800/Mandelbrot/blob/master/testpics/optim3_1.png)\
заменились на\
![optim4](https://github.com/ThreadJava800/Mandelbrot/blob/master/testpics/optim4.png).\
По сравнению с прошлым шагом, эта оптимизация ускорила алгоритм в 20 раз быстрее, чем на 4 шаге (то есть, время стало тем же, что и на [2 шаге](#step2)).\
Здесь [`_mm_mul_ps`](https://www.laruence.com/sse/#text=_mm_mul_ps&expand=3928) и [`_mm_add_ps`](https://www.laruence.com/sse/#text=_mm_add_ps&expand=3928,133) - <em>intrinsic функции</em>.

### Шаг 6
Далее, для ускорения программы было принято решение переключиться на [AVX инструкции](https://github.com/ThreadJava800/Mandelbrot/blob/master/optim5.cpp), которые работают с 8 float числами за раз.\
Алгоритм стал работать ~ 2 раза быстрее по сравнению с прошлым шагом.

## Сравнение реализаций <a name="compare"></a>
Важно отметить, что во время тестирования компьютер был подключён к сети и никаких других приложений не было запущено.

Все расчёты были запущены 100 раз. По итогу было взято среднее время.

*Время работы измерялось лишь для расчётов (графика и выделение памяти не считалось)*


Для начала, все программы были собраны с флагом `-O2`:


| Реализация | Теоретическое ускорение | Фактическое ускорение |
| --- | --- | --- |
| `Наивная` | 1x (437.5ms) | 1x (437.5 ± 0.65 ms) |
| `SSE4.2` | 4x (109.38ms)  | 3.78x (115.7 ± 0.66 ms) |
| `AVX256` | 8x (54.69ms) | 6.9x (63.16 ± 0.6ms) |

`-O3`:

| Реализация | Теоретическое ускорение | Фактическое ускорение |
| --- | --- | --- |
| `Наивная` | 1x (437.5ms) | 1x (437.5 ± 0.64 ms) |
| `SSE4.2` | 4x (109.38ms)  | 3.79x (115.5 ± 0.38 ms) |
| `AVX256` | 8x (54.69ms) | 7.3x (60 ± 0.38 ms) | 

`-Ofast`:

| Реализация | Теоретическое ускорение | Фактическое ускорение |
| --- | --- | --- |
| `Наивная` | 1x (406.17ms) | 1x (406.17 ± 0.51 ms) |
| `SSE4.2` | 4x (101.54ms)  | 3.9x (104.24 ± 0.41 ms) |
| `AVX256` | 8x (50.77ms) | 7.2x (56.4 ± 0.39 ms) |

## Углубимся в ассемблер <a name="assemble"></a>
Я решил исследовать, как работают различные оптимизации `g++`.

`-O1` работает с локальными переменными через регистры, в отличии от `-O0`, который использует стек.\
Это рабоает быстрее, т.к. нет надобности в пересылке данных из регистра в стек и наоборот.

Ссылка на [godbolt.org](https://godbolt.org/z/db9YT7hKj)

Код на си:\
![C code](https://github.com/ThreadJava800/Mandelbrot/blob/master/testpics/c_code.png#center)\
Ассемблерный код (`-O0`):\
![O0](https://github.com/ThreadJava800/Mandelbrot/blob/master/testpics/o0.png#center)\
Ассемблерный код (`-O1`):\
![O1](https://github.com/ThreadJava800/Mandelbrot/blob/master/testpics/o1.png)

`-O2` переставляет команды таким образом, что вычисления происходят быстрее (благодаря вычислительному конвееру).

*В регистре `eax` хранится переменная цикла, которая увеличивается на единицу каждую итерацию.*

Код на си:\
![C code](https://github.com/ThreadJava800/Mandelbrot/blob/master/testpics/c_code2.png#center)\
Ассемблерный код (`-O1`):\
![O1](https://github.com/ThreadJava800/Mandelbrot/blob/master/testpics/o12.png#center)\
Ассемблерный код (`-O2`):\
![O2](https://github.com/ThreadJava800/Mandelbrot/blob/master/testpics/ofast2.png)

Другие флаги оптимизации (`-O3` и `-Ofast`) не повлияли на ассемблерный код.

## Заключение <a name="conclusion"></a>
Использование <em>SIMD инструкций</em> способно в разы ускорить алгоритм. Учитывая, что написание алгоритма с использованием <em>intrinsic</em> функций не трудозатратно, в будущем я буду использовать эти оптимизации при необходимости.