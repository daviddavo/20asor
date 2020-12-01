#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "daverror.h"

int main() {
    int uid, gid;

    printf("uid: %ld\n", DAVO_CHECK(uid = getuid()));
    printf("euid: %ld\n", DAVO_CHECK(gid = geteuid()));

    if (gid != uid) {
        fprintf(stderr, "Warning, setuid used");
    }

    return 0;
}
