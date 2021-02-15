/* SPDX-License-Identifier: MIT */

#include <infos.h>

static HFILE vc;

struct Args {
    int widthFrom;
    int widthTo;
};

#define BLACK 0
#define BLUE 2
#define GREEN 3
#define CYAN 4
#define RED 4
#define MAGENTA 5
#define ORANGE 6
#define L_GREY 7
#define GREY 8
#define L_BLUE 9
#define L_GREEN 10
#define L_CYAN 11
#define L_RED 12
#define L_MAGENTA 13
#define YELLOW 14
#define WHITE 15

#define MAXITERATE 10000000
#define NORM_FACT 67108864
#define NORM_BITS 26

int64_t realMin, realMax;
int64_t imagMin, imagMax;
int64_t deltaReal, deltaImag;

int width = 80; // frame is 80x25
int height = 25;

static void drawchar(int x, int y, int attr, unsigned char c) {
    uint16_t u = (attr << 8) | c;
    pwrite(vc, (const char *)&u, sizeof(u), x + (y * 80));
}

void output(int value, int i, int j) {
    if (value == 10000000) {drawchar(j, i, BLACK, ' ');}
    else if (value > 9000000) {drawchar(j, i, RED, '*');}
    else if (value > 5000000) {drawchar(j, i, L_RED, '*');}
    else if (value > 1000000) {drawchar(j, i, ORANGE, '*');}
    else if (value > 500) {drawchar(j, i, YELLOW, '*');}
    else if (value > 100) {drawchar(j, i, L_GREEN, '*');}
    else if (value > 10) {drawchar(j, i, GREEN, '*');}
    else if (value > 5) {drawchar(j, i, L_CYAN, '*');}
    else if (value > 4) {drawchar(j, i, CYAN, '*');}
    else if (value > 3) {drawchar(j, i, L_BLUE, '*');}
    else if (value > 2) {drawchar(j, i, BLUE, '*');}
    else if (value > 1) {drawchar(j, i, MAGENTA, '*');}
    else {drawchar(j, i, L_MAGENTA, '*');}
}

static void mandelbrot(void *arg) {
    Args args = *((Args *) arg);
    int widthFrom = args.widthFrom;
    int widthTo = args.widthTo;

    for (int i = 0; i < height; i++) {
        for (int j = widthFrom; j < widthTo; j++) {
            int64_t real0, imag0, realq, imagq, real, imag;
            int count;

            real0 = realMin + j*deltaReal; // current real value
            imag0 = imagMax - i*deltaImag;

            real = real0;
            imag = imag0;
            for (count = 0; count < MAXITERATE; count++) {
                realq = (real * real) >> NORM_BITS;
                imagq = (imag * imag) >> NORM_BITS;

                if ((realq + imagq) > ((int64_t) 4 * NORM_FACT)) break;

                imag = ((real * imag) >> (NORM_BITS-1)) + imag0;
                real = realq - imagq + real0;
            }

            output(count, i, j);
        }
    }

    stop_thread(HTHREAD_SELF);
}

int main(const char *cmdline) {
    vc = open("/dev/vc0", 0);

    if (is_error(vc)) {
        printf("error: unable to open vc");
        return 1;
    }

    printf("\033\x09How many threads would you like to use?\n");
//    int numThreads = getch();
    int numThreads = 16;
    HTHREAD threads[numThreads];

    realMin = -2 * NORM_FACT;
    realMax = 1 * NORM_FACT;
    imagMin = -1 * NORM_FACT;
    imagMax = 1 * NORM_FACT;

    deltaReal = (realMax - realMin)/(width - 1);
    deltaImag = (imagMax - imagMin)/(height - 1);

    int widthShared = width / numThreads;

    Args args[numThreads];

    for (unsigned int k = 0; k < numThreads; k++) {
        Args *arg = &args[k];

        // In the case that the screen width doesn't divide evenly by
        // the number of threads, the last thread takes any extra
        if (k == numThreads - 1) {
            arg->widthFrom = k*widthShared;
            arg->widthTo = width;
        } else {
            arg->widthFrom = k*widthShared;
            arg->widthTo = (k+1)*widthShared;
        }

        threads[k] = create_thread(mandelbrot, arg);
    }

    for (unsigned int k = 0; k < numThreads; k++) {
        join_thread(threads[k]);
    }

    close(vc);
    // wait for input so the prompt doesn't ruin the lovely image
    // remove this when timing!
    getch();
    return 0;
}