#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/time.h>

#include "daverror.h"

#define LOOP_LIMIT 10000000

int main() {
    timeval tstart, tend;
    suseconds_t total;
    int var = 0;

    DAVO_CHECK(gettimeofday(&tstart, NULL));
    for (int i = 0; i < LOOP_LIMIT; i++) { var++; }
    DAVO_CHECK(gettimeofday(&tend, NULL));

    total = (tend.tv_sec - tstart.tv_sec)*1000000;
    total += tend.tv_usec - tstart.tv_usec;

    printf("Total: %ldus\n", total);

    return 0;
}
