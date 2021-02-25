/* SPDX-License-Identifier: MIT */

#include <infos.h>

#define MAXITERATE 10000000
#define NORM_FACT 67108864
#define NORM_BITS 26

int64_t realMin, realMax;
int64_t imagMin, imagMax;
int64_t deltaReal, deltaImag;

int width = 80; // frame is 80x25
int height = 25;

int next_pixel = 0;
int *next_pixel_ptr = &next_pixel;
int last_pixel;

int display[80][25];

static void mandelbrot(void *arg) {
    int my_next_pixel = fetch_and_add(next_pixel_ptr, 1);

    while (my_next_pixel <= last_pixel) {
        int x = my_next_pixel % width;
        int y = my_next_pixel / width;

        int64_t real0, imag0, realq, imagq, real, imag;
        int count;

        real0 = realMin + x*deltaReal; // current real value
        imag0 = imagMax - y*deltaImag;

        real = real0;
        imag = imag0;
        for (count = 0; count < MAXITERATE; count++) {
            realq = (real * real) >> NORM_BITS;
            imagq = (imag * imag) >> NORM_BITS;

            if ((realq + imagq) > ((int64_t) 4 * NORM_FACT)) break;

            imag = ((real * imag) >> (NORM_BITS-1)) + imag0;
            real = realq - imagq + real0;
        }

        display[x][y] = count;
        my_next_pixel = fetch_and_add(next_pixel_ptr, 1);
    }

    stop_thread(HTHREAD_SELF);
}

int main(const char *cmdline) {
    int numThreads = 2;
    HTHREAD threads[numThreads];

    realMin = -2 * NORM_FACT;
    realMax = 1 * NORM_FACT;
    imagMin = -1 * NORM_FACT;
    imagMax = 1 * NORM_FACT;

    deltaReal = (realMax - realMin)/(width - 1);
    deltaImag = (imagMax - imagMin)/(height - 1);

    last_pixel = width * height;

    for (unsigned int k = 0; k < numThreads; k++) {
        threads[k] = create_thread(mandelbrot, (void *) 1);
    }

    for (unsigned int k = 0; k < numThreads; k++) {
        join_thread(threads[k]);
    }

//    for (int i = 0; i < width; i++) {
//        for (int j = 0; j < height; j++) {
//            printf(" %u", display[i][j]);
//        }
//        printf("\n");
//    }

    return 0;
}