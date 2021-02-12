/* SPDX-License-Identifier: MIT */

#include <infos.h>

struct Args {
    int64_t x;
    int64_t y;
};

char *black = "\033\x00*";
char *blue = "\033\x01*";
char *green = "\033\x02*";
char *cyan = "\033\x03*";
char *red = "\033\x04*";
char *magenta = "\033\x05*";
char *orange = "\033\x06*";
char *l_grey = "\033\x07*";
char *grey = "\033\x08*";
char *l_blue = "\033\x09*";
char *l_green = "\033\x0a*";
char *l_cyan = "\033\x0b*";
char *l_red = "\033\x0c*";
char *l_magenta = "\033\x0d*";
char *yellow = "\033\x0e*";
char *white = "\033\x0f*";

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

        if ((realq + imagq) > ((int64_t) 4 * NORM_FACT)) return i;

        imag = ((real * imag) >> (NORM_BITS-1)) + imag0;
        real = realq - imagq + real0;
    }

    return i;
}

void output(int value) {
    if (value == 100) {printf(" ");}
    else if (value > 90) {printf(red);}
    else if (value > 70) {printf(l_red);}
    else if (value > 50) {printf(orange);}
    else if (value > 30) {printf(yellow);}
    else if (value > 20) {printf(l_green);}
    else if (value > 10) {printf(green);}
    else if (value > 5) {printf(l_cyan);}
    else if (value > 4) {printf(cyan);}
    else if (value > 3) {printf(l_blue);}
    else if (value > 2) {printf(blue);}
    else if (value > 1) {printf(magenta);}
    else {printf(l_magenta);}
}

int main(const char *cmdline) {

    // frame is 80 x 25
    int width = 80; // number of characters fitting horizontally on my screen
    int height = 24; // number of characters fitting vertically on my screen

    int64_t realMin = -2 * NORM_FACT;
    int64_t realMax = 1 * NORM_FACT;
    int64_t imagMin = -1 * NORM_FACT;
    int64_t imagMax = 1 * NORM_FACT;

    int64_t deltaReal = (realMax - realMin)/(width - 1);
    int64_t deltaImag = (imagMax - imagMin)/(height - 1);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            // todo: split this work among threads

            Args args = Args();
            args.x = realMin + j*deltaReal; // current real value
            args.y = imagMax - i*deltaImag;

            int value = mandelbrot(&args);
            output(value);
        }
    }

    return 0;
}