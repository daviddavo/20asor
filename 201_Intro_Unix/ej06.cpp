#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "daverror.h"

int main() {
    printf("Maximum length of arguments: %ld\n", DAVO_CHECK(sysconf(_SC_ARG_MAX)));
    printf("Maximum number of process per user ID: %ld\n", DAVO_CHECK(sysconf(_SC_CHILD_MAX)));
    printf("Maximum number of files: %ld\n", DAVO_CHECK(sysconf(_SC_OPEN_MAX)));
    printf("Posix version: %ld\n", DAVO_CHECK(sysconf(_SC_VERSION)));

    return 0;
}
