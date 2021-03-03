/* SPDX-License-Identifier: MIT */

#include <infos.h>

int array[1024]; // int 4 bytes, so each cache has 1/4 of array which is 1kb
int sum;
int salt;

static void thread_proc(void *arg)
{
	unsigned int thread_num = (unsigned int)(unsigned long)arg;
	int hash = 0;

    while (hash < sum+4000000) { // Each thread iterates 1,000,000 times
        hash = salt;
        for (int i = thread_num*256; i < (thread_num+1)*256; i++) {
            hash += array[i];
        };

        // Each thread increments the salt, so each thread will
        // iterate a minimum amount of times before meeting the condition
        // since all we want is the thread to keep iterating
        fetch_and_add(&salt, 1);
        yield(HTHREAD_SELF);
    }

	printf("Suitable hash found %u!\n", hash);
	stop_thread(HTHREAD_SELF);
}

int main(const char *cmdline)
{
	printf("Running Cache Test!\n");
    salt = 0;
    sum = 524800;

//	 Initialise array with values 1 to 1024, sum is 524,800
	for (int i = 0; i < 1024; i++) {
        array[i] = i+1;
    }

	HTHREAD threads[4];
	printf("Using %d threads...\n", ARRAY_SIZE(threads));
	for (unsigned int i = 0; i < ARRAY_SIZE(threads); i++) {
		threads[i] = create_thread(thread_proc, (void *)(unsigned long)i);
	}

	for (unsigned int i = 0; i < ARRAY_SIZE(threads); i++) {
		printf("Waiting for thread %d...\n", i);
		join_thread(threads[i]);
	}

	printf("Cache Test Complete\n");
	return 0;
}
