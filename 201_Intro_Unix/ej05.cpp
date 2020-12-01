#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <sys/utsname.h>

int main() {
    utsname info;

    if (uname(&info)) {
        printf("%d: %s\n", errno, strerror(errno));
    }

    printf("%s %s %s %s %s %s\n",
        info.sysname,
        info.nodename,
        info.release,
        info.version,
        info.machine,
        info.domainname
    );

    return 0;
}
