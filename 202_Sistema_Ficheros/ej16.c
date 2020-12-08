#define _XOPEN_SOURCE 500

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "daverror.h"

#define SLEEP_TIME 30

int main(int argc, char ** argv) {
    int fd;

    if (argc < 2) return EXIT_FAILURE;

    DAVO_CHECK(fd = open(argv[1], O_WRONLY));

    if (lockf(fd, F_TEST, 0)) {
        if (errno == EACCES) {
            printf("El fichero %s está bloqueado\n", argv[1]);
            return EXIT_SUCCESS;
        } else {
            DAVO_CHECK(-1); 
            return EXIT_FAILURE;
        }
    } 

    DAVO_CHECK(lockf(fd, F_TLOCK, 0));
    printf("He bloqueado el fichero %s\n", argv[1]);
    for (int i = SLEEP_TIME; i > 0; i--) {
        printf("\rDurmiendo, %ds restante(s) ", i);
        fflush(stdout);
        sleep(1);
    }
    DAVO_CHECK(lockf(fd, F_ULOCK, 0));

    return EXIT_SUCCESS;
}
