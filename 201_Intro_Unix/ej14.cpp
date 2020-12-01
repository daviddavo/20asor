#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>

#include "daverror.h"

#define LOOP_LIMIT 10000000

int main() {
    time_t t;
    tm * tm;

    DAVO_CHECK(t = time(NULL));

    tm = localtime(&t);

    printf("Year: %d\n", tm->tm_year+1900);

    return 0;
}
