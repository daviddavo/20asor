#include <unistd.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>

#include "daverror.h"

/* Escribir un programa que bloquee las señales SIGINT y SIGTSTP. Después de
 * bloquearlas el programa debe suspender su ejecución con sleep(3) un número
 * de segundos que se obtendrán de la variable de entorno SLEEP_SECS.
 * Después de despertar de sleep(3), el proceso debe informar de si recibió la
 * señal SIGINT y/o SIGTSTP. En este último caso, debe desbloquearla con lo
 * que el proceso se detendrá y podrá ser reanudado en la shell (imprimir una
 * cadena antes de finalizar el programa para comprobar este comportamiento).
 */

// gcc ej11.cpp && ./a.out & sleep 1; kill -TSTP $! ; echo 


int main(int argc, char ** argv) {
    sigset_t blk_set, pending_set;
    char * strseconds;
    int seconds;

    strseconds = getenv("SLEEP_SECS");

    if (strseconds == NULL) {
        fprintf(stderr, "SLEEP_SECS not specified");
        return EXIT_FAILURE;
    }

    seconds = atoi(strseconds);

    printf("Sleeping for %ds\n", seconds);

    sigemptyset(&blk_set);
    sigaddset(&blk_set, SIGINT);
    sigaddset(&blk_set, SIGTSTP);
    sigprocmask(SIG_BLOCK, &blk_set, NULL);

    sleep(seconds);

    DAVO_CHECK(sigpending(&pending_set));
    if (sigismember(&pending_set, SIGINT)) {
        printf("Recibida señal SIGINT\n");
    } else if (sigismember(&pending_set, SIGTSTP)) {
        printf("Recibida señal SIGTSTP\n");
    }

    sigprocmask(SIG_UNBLOCK, &blk_set, NULL);

    return EXIT_SUCCESS;
}
