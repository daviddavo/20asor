#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "daverror.h"

int main() {
    printf("Max links: %ld\n", DAVO_CHECK(pathconf(".", _PC_LINK_MAX)));
    printf("Max path: %ld\n", DAVO_CHECK(pathconf(".", _PC_PATH_MAX)));
    printf("Max fname: %ld\n", DAVO_CHECK(pathconf(".", _PC_NAME_MAX)));

    return 0;
}
