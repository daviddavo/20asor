#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define ERR_LIMIT 255

int main() {

    for (int i = 0; i < ERR_LIMIT; i++) {
        printf("%d: %s\n", i, strerror(i));
    }

    return 0;
}
