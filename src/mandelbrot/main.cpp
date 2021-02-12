/* SPDX-License-Identifier: MIT */

#include <infos.h>

static HFILE vc;

struct Args {
    int threadNum;
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

#define MAXITERATE 100
#define NORM_FACT 8192
#define NORM_BITS 13

int64_t realMin = -2 * NORM_FACT;
int64_t realMax = 1 * NORM_FACT;
int64_t imagMin = -1 * NORM_FACT;
int64_t imagMax = 1 * NORM_FACT;
int64_t deltaReal, deltaImag;

int width = 80; // frame is 80x25
int height = 20; // todo: change back to 25
int heightShared, widthShared;

static void drawchar(int x, int y, int attr, unsigned char c)
{
    uint16_t u = (attr << 8) | c;
    pwrite(vc, (const char *)&u, sizeof(u), x + (y * 80));
}

void output(int value, int i, int j) {
    if (value == 100) {drawchar(j, i, BLACK, ' ');}
    else if (value > 90) {drawchar(j, i, RED, '*');}
    else if (value > 70) {drawchar(j, i, L_RED, '*');}
    else if (value > 50) {drawchar(j, i, ORANGE, '*');}
    else if (value > 30) {drawchar(j, i, YELLOW, '*');}
    else if (value > 20) {drawchar(j, i, L_GREEN, '*');}
    else if (value > 10) {drawchar(j, i, GREEN, '*');}
    else if (value > 5) {drawchar(j, i, L_CYAN, '*');}
    else if (value > 4) {drawchar(j, i, CYAN, '*');}
    else if (value > 3) {drawchar(j, i, L_BLUE, '*');}
    else if (value > 2) {drawchar(j, i, BLUE, '*');}
    else if (value > 1) {drawchar(j, i, MAGENTA, '*');}
    else {drawchar(j, i, L_MAGENTA, '*');}
}

static void mandelbrot(void *arg) {
    int threadNum = *((int *) arg);

    for (int i = 0; i < heightShared; i++) {
        for (int j = 0; j < widthShared; j++) {
            int64_t real0, imag0, realq, imagq, real, imag;
            int count;

            real0 = realMin + j*threadNum*deltaReal; // current real value
            imag0 = imagMax - i*threadNum*deltaImag;

            real = real0;
            imag = imag0;
            for (count = 0; count < MAXITERATE; count++) {
                realq = (real * real) >> NORM_BITS;
                imagq = (imag * imag) >> NORM_BITS;

                if ((realq + imagq) > ((int64_t) 4 * NORM_FACT)) break;

                imag = ((real * imag) >> (NORM_BITS-1)) + imag0;
                real = realq - imagq + real0;
            }

            output(count, i*threadNum, j*threadNum);
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

    // todo: get from user
    int numThreads = 4; // todo: what if it doesn't divide nicely
    HTHREAD threads[numThreads];

    heightShared = height / numThreads;
    widthShared = width / numThreads;

    deltaReal = (realMax - realMin)/(width - 1);
    deltaImag = (imagMax - imagMin)/(height - 1);

    for (unsigned int k = 0; k < numThreads; k++) {
        threads[k] = create_thread(mandelbrot, &k);
    }

    for (unsigned int i = 0; i < numThreads; i++) {
        join_thread(threads[i]);
    }

    close(vc);
    // wait for input so the prompt doesn't ruin the lovely image
    getch();
    return 0;
}