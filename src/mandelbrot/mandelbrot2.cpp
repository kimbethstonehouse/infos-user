/* SPDX-License-Identifier: MIT */

#include <infos.h>

struct Args {
    int64_t x;
    int64_t y;
};

#define MAXITERATE 100
#define NORM_FACT 8192
#define NORM_BITS 13

int mandelbrot(void *arg) {
    Args args = *((Args *) arg);

    int64_t real0 = args.x;
    int64_t imag0 = args.y;

    int64_t realq, imagq, real, imag;
    int i;

    real = real0;
    imag = imag0;
    for (i = 0; i < MAXITERATE; i++) {
        realq = (real * real) >> NORM_BITS;
        imagq = (imag * imag) >> NORM_BITS;

        if ((realq + imagq) > (int64_t) 4 * NORM_FACT) return i;

        imag = ((real * imag) >> (NORM_BITS-1)) + imag0;
        real = realq - imagq + real0;
    }

    return i;
}

int main(const char *cmdline) {

    int width = 20; // number of characters fitting horizontally on my screen
    int heigth = 15; // number of characters fitting vertically on my screen

    int64_t realMin = -2 * NORM_FACT;
    int64_t realMax = 1 * NORM_FACT;
    int64_t imagMin = -1 * NORM_FACT;
    int64_t imagMax = 1 * NORM_FACT;

    int64_t deltaReal = (realMin - realMax)/(width - 1);
    int64_t deltaImag = (imagMax - imagMin)/(heigth - 1);

    for (int i = 0; i < heigth; i++) {
        for (int j = 0; j < width; j++) {
            // todo: split this work among threads

            Args args = Args();
            args.x = realMin + j*deltaReal; // current real value
            args.y = imagMax - i*deltaImag;

            int value = mandelbrot(&args);

            // todo: print value
        }
        printf("\n");
    }

    return 0;
}