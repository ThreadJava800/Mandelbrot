# Визуализация множества мандельброта
> Визуализация множества мандельброта с SIMD оптимизациями.

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

Я исследовал влияние интринсиков на быстродействие этого несложного алгоритма.\
Также, моей целью было ближе познакомиться с понятием `параллелизма уровня инструкций`.

## Ход работы <a name="progress"></a>

### Шаг 1
[Простая (наивная) реализация](https://github.com/ThreadJava800/Mandelbrot/blob/master/naive.cpp)

### Шаг 2
В ручную добавил [развёртку циклов](https://github.com/ThreadJava800/Mandelbrot/blob/master/optim1.cpp).\
Работает гораздо быстрее, но код стал не читаемым.

### Шаг 3
Решил сократить количество кода с помощью [циклов](https://github.com/ThreadJava800/Mandelbrot/blob/master/optim2.cpp) в надежде, что `g++` сумеет развернуть их. Но, он не смог :)\
`g++` без флагов оптимизации не рискует разворачивать циклы\
В результате, программа стала работать даже дольше, чем наивная реализация.

### Шаг 4
Затем я решил заменить циклы на [`inline` функции](https://github.com/ThreadJava800/Mandelbrot/blob/master/optim3.cpp), ведь их компилятор с большей вероятностью свернёт.\
Однако, на скорость работы программы это не повлияло.

### Шаг 5
Затем, я решил использовать [интринсики](https://github.com/ThreadJava800/Mandelbrot/blob/master/optim4.cpp), в частности, `SSE` оптимизации.\
По сравнению с прошлым шагом, эта оптимизация ускорила алгоритм в 20 раз.

### Шаг 6
Далее, для ускорения программы было принято решение переключиться на [`AVX` инструкции](https://github.com/ThreadJava800/Mandelbrot/blob/master/optim5.cpp), которые работают с 8 float числами за раз.\
Алгоритм стал работать ~ 2 раза быстрее.

### Шаг 7
Затем, была написана реализация алгоритма для [AVX512 оптимизаций](https://github.com/ThreadJava800/Mandelbrot/blob/master/optim6.cpp)
Then I switched to __m512` with 16 floats in it.\
Но оказалось, что мой процессора не поддерживает данные инструкции.

## Сравнение реализаций <a name="compare"></a>
Важно отметить, что во время тестирования компьютер был подключён к сети и никаких других приложений не было запущено.

Все расчёты были запущены 100 раз. По итогу было взято среднее время.

*Время работы измерялось лишь для расчётов (графика и считывание картинок из памяти не считались)*


Для начала, все программы были собраны с флагом `-O2`:


| Реализация | Теоретическое ускорение | Фактическое ускорение |
| --- | --- | --- |
| `Наивная` | 1x (437.5ms) | 1x (437.5 ± 0.65 ms) |
| `SSE4.2` | 4x (109.38ms)  | 3.78x (115.7 ± 0.66 ms) |
| `AVX256` | 8x (54.69ms) | 6.9x (63.16 ± 0.6ms) |

-O3:

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

Код на си:\
![C code](https://github.com/ThreadJava800/Mandelbrot/blob/master/testpics/c_code.png#center)\
Ассемблерный код (`-O0`):\
![O0](https://github.com/ThreadJava800/Mandelbrot/blob/master/testpics/o0.png#center)\
Ассемблерный код (`-O1`):\
![O1](https://github.com/ThreadJava800/Mandelbrot/blob/master/testpics/o1.png)

`-O2` переставляет команды таким образом, что вычисления происходят быстрее (благодаря вычислительному конвееру).

Код на си:\
![C code](https://github.com/ThreadJava800/Mandelbrot/blob/master/testpics/c_code2.png#center)\
Ассемблерный код (`-O1`):\
![O1](https://github.com/ThreadJava800/Mandelbrot/blob/master/testpics/o12.png#center)\
Ассемблерный код (`-O2`):\
![O2](https://github.com/ThreadJava800/Mandelbrot/blob/master/testpics/ofast2.png)

Другие флаги оптимизации (`-O3` и `-Ofast`) не повлияли на ассемблерный код.

## Заключение <a name="conclusion"></a>
Использование `SIMD инструкций` способно в разы ускорить время работы алгоритма. Учитывая, что написания алгоритма с использованием интринсиков не трудозатратно, в будущем я буду использовать эти оптимизации, если необходимо.