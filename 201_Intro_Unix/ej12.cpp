#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>

#include "daverror.h"

int main() {
    time_t t;

    DAVO_CHECK(t = time(NULL));
    printf("%ld\n", t);

    return 0;
}
