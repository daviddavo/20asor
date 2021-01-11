#include <unistd.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>

#include "daverror.h"

/* Escribir un programa que realice el borrado programado del propio ejecutable.
 * El programa tendrá como argumento el número de segundos que esperará antes
 * de borrar el fichero. El borrado del fichero se podrá detener si se recibe
 * la señal SIGUSR1.
 */

volatile bool borrar = true;

void sighandler(int signal) {
    if (signal == SIGUSR1) {
        borrar = false;
    }
}

int main(int argc, char ** argv) {
    int seconds;

    if (argc != 2) {
        fprintf(stderr, "Especifica el número de segundos a esperar\n");
        return EXIT_FAILURE;
    }

    seconds = atoi(argv[1]);

    if (seconds == 42) printf("%d\n", getpid());

    struct sigaction sa {};

    sa.sa_handler = sighandler;

    DAVO_CHECK(sigaction(SIGUSR1,  &sa, NULL));

    for (int i = seconds; i > 0 && borrar; i--) {
        printf("\rTe quedan %d segundos ", i);
        fflush(stdout);
        sleep(1);
    }

    printf("\r                         \r");
    fflush(stdout);

    if (borrar) {
        printf("Borrando %s\n", argv[0]);
        DAVO_CHECK(remove(argv[0]));
    } else {
        printf("Te has salvado\n");
    }

    return EXIT_SUCCESS;
}
