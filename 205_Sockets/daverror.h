#ifndef DAVO_CHECK
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

long __davo_check(const long n, const char f[], const int l, int cmp) {
    if (n == cmp) {
        fprintf(stderr, "Error: %s (%d)\n", strerror(errno), errno);
        fprintf(stderr, "@ %s:%d\n", f, l);
        exit(1);
    }

    return n;
}

#define DAVO_CHECK(n) __davo_check(n, __FILE__, __LINE__, -1)
#define DAVO_CHECK0(n) __davo_check(n, __FILE__, __LINE__, 0)

#endif
