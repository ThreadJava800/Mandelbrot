#include <iostream>
#include <complex>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <omp.h>

using namespace std;

int main(int argc, char **argv) {
    double xmin, xmax, ymin, ymax, dx, dy;
    int N, M, maxit, i, j, k;
    unsigned char *data;

    if (argc < 6) {
        cerr << "Usage: " << argv[0] << " <xmin> <xmax> <ymin> <ymax> <N> <maxit>" << endl;
        return 1;
    }

    xmin = atof(argv[1]);
    xmax = atof(argv[2]);
    ymin = atof(argv[3]);
    ymax = atof(argv[4]);
    N = atoi(argv[5]);
    maxit = atoi(argv[6]);

    dx = (xmax - xmin) / N;
    dy = (ymax - ymin) / N;

    data = new unsigned char[N * N * 3];

    clock_t startTime = clock();
    #pragma omp parallel for collapse(2) schedule(dynamic, 16) shared(data, N, maxit, xmin, dx, ymin, dy)
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            complex<double> c(xmin + i * dx, ymin + j * dy);
            complex<double> z(0);
            int n = 0;

            while (abs(z) < 10 && n < maxit) {
                z = z*z + c;
                n++;
            }

            if (n == maxit) {
                data[(i * N + j) * 3] = 0;
                data[(i * N + j) * 3 + 1] = 0;
                data[(i * N + j) * 3 + 2] = 0;
            } else {
                data[(i * N + j) * 3] = 8 * log(n);
                data[(i * N + j) * 3 + 1] = 16 * log(n);
                data[(i * N + j) * 3 + 2] = 32 * log(n);
            }
        }
    }
    printf("%lf", ((double)clock() - (double)startTime) / CLOCKS_PER_SEC * 1000);

    ofstream fout("mandelbrot.ppm");
    fout << "P3 " << N << " " << N << " " << "255" << endl;
    for (i = 0; i < N * N * 3; i++) {
        fout << (int) data[i] << " ";
    }
    fout.close();

    delete[] data;

    return 0;
}