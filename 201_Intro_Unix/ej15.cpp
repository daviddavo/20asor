#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <langinfo.h>

#include "daverror.h"

#define STR_MAX 255

int main() {
    time_t t;
    tm * tm;
    char str[STR_MAX];
    char * strfmt;
    size_t str_size;

    DAVO_CHECK(t = time(NULL));
    strfmt = nl_langinfo(D_T_FMT);

    tm = localtime(&t);
    str_size = strftime(str, STR_MAX, strfmt, tm);
    puts(str);

    return 0;
}
