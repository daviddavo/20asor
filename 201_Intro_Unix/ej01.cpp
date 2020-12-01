#include <stdio.h>
#include <unistd.h>
#include <errno.h>

int main() {
    if (setuid(0)) {
        return 1;
    }

    return 0;
}
