#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <pwd.h>

#include "daverror.h"

int main() {
    int uid, gid;
    passwd * passwd;

    printf("uid: %ld\n", DAVO_CHECK(uid = getuid()));
    printf("euid: %ld\n", DAVO_CHECK(gid = geteuid()));

    if (gid != uid) {
        fprintf(stderr, "Warning, setuid used");
    }

    passwd = getpwuid(uid);

    printf("user name: %s\n", passwd->pw_name);
    printf("home: %s\n", passwd->pw_dir);
    printf("description: %s\n", passwd->pw_gecos);

    return 0;
}
