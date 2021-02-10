/* SPDX-License-Identifier: MIT */

#include <infos.h>

struct Args
{
    double x;
    double y;
};

static int mandelbrot(void *arg) {
    Args args = *((Args *) arg);
    double real = args.x;
    double imag = args.y;

    int limit = 100;
    double zReal = real;
    double zImag = imag;

    for (int i = 0; i < limit; ++i) {
        double r2 = zReal * zReal;
        double i2 = zImag * zImag;

        if (r2 + i2 > 4.0) return i;

        zImag = 2.0 * zReal * zImag + imag;
        zReal = r2 - i2 + real;
    }
    return limit;
}

int main(const char *cmdline) {

    int width = 20; //number of characters fitting horizontally on my screen
    int heigth = 15; //number of characters fitting vertically on my screen

    double x_start = -2.0;
    double x_fin = 1.0;
    double y_start = -1.0;
    double y_fin = 1.0;

    //~ double x_start = -0.25;
    //~ double x_fin = 0.05;
    //~ double y_start = -0.95;
    //~ double y_fin = -0.75;

    //~ double x_start = -0.13;
    //~ double x_fin = -0.085;
    //~ double y_start = -0.91;
    //~ double y_fin = -0.88;

    double dx = (x_fin - x_start)/(width - 1);
    double dy = (y_fin - y_start)/(heigth - 1);

    char * char_ = "\u2588";

    char *black = "\033[22;30m\u2588";
    char *red = "\033[22;31m\u2588";
    char *l_red = "\033[01;31m\u2588";
    char *green = "\033[22;32m\u2588";
    char *l_green = "\033[01;32m\u2588";
    char *orange = "\033[22;33m\u2588";
    char *yellow = "\033[01;33m\u2588";
    char *blue = "\033[22;34m\u2588";
    char *l_blue = "\033[01;34m\u2588";
    char *magenta = "\033[22;35m\u2588";
    char *l_magenta = "\033[01;35m\u2588";
    char *cyan = "\033[22;36m\u2588";
    char *l_cyan = "\033[01;36m\u2588";
    char *gray = "\033[22;37m\u2588";
    char *white = "\033[01;37m\u2588";

//    HTHREAD threads[20][15];

    for (int i = 0; i < heigth; i++) {
        for (int j = 0; j < width; j++) {

            Args args = Args();
            args.x = x_start + j*dx; // current real value
            args.y = y_fin - i*dy;

            int value = mandelbrot(&args);
//            threads[i][j] = create_thread(mandelbrot, (void *) xy);

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

//            printf("\033[0m");

//            join_thread(threads[i][j]);
        }
        printf("\n");
    }

    return 0;
}