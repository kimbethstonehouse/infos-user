/* SPDX-License-Identifier: MIT */

#include <infos.h>

struct Parameters {
    double exponent;
    double base;
};

static void power(void *arg)
{
    Parameters p = *((Parameters *) arg);
    double j = p.exponent;

    char name_buffer[16] = {0};
    sprintf(name_buffer, "thread/%d", (uint64_t) p.exponent);
    set_thread_name(HTHREAD_SELF, name_buffer);

    double result;
    for (int i = 0; i < 100000; i++) {
        result = p.base;
        j = p.exponent;
        while (j > 0) {
            result = result * p.base;
            j--;
//        usleep(1000);
        }
    }

//    usleep(1000000);
    printf("Arg exponent %f, end exponent %f, result is: %f\n", *(volatile double *) arg, p.exponent, result);
    stop_thread(HTHREAD_SELF);
}

int main(const char *cmdline)
{
    printf("Running Scheduler Test 3!\n");

    double x = 1.4;
    double y = 0.3;

    Parameters p1[10];
    for (int i = 0; i < 10; i++) {
        p1[i].base = 2;
        p1[i].exponent = i+1;
    }

    HTHREAD threads[10];

    for (int i = 0; i < 10; i++) {
        threads[i] = create_thread(power, (void *) &p1[i]);
//        usleep(1000000);
    }

    for (int i = 0; i < 10; i++) {
        join_thread(threads[i]);
    }

    printf("Scheduler Test Complete\n");

    return 0;
}