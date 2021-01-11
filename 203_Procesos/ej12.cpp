#include <unistd.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>

#include "daverror.h"

/* Escribir un programa que instale un manejador sencillo para las señales
 * SIGINT y SIGTSTP. El manejador debe contar las veces que ha recibido cada
 * señal. El programa principal permanecerá en un bucle que se detendrá cuando
 * se hayan recibido 10 señales. El número de señales de cada tipo se mostrará
 * al finalizar el programa. 
 */

volatile unsigned int nint = 0;
volatile unsigned int nstp = 0;

void sighandler(int signal) {
    switch (signal) {
        case SIGINT: nint++; break;
        case SIGTSTP: nstp++; break;
    }
}

int main(int argc, char ** argv) {
    struct sigaction sa {};

    sa.sa_handler = sighandler;

    DAVO_CHECK(sigaction(SIGINT,  &sa, NULL));
    DAVO_CHECK(sigaction(SIGTSTP, &sa, NULL));

    while (nint + nstp < 10) {
        // Para no mantener al procesador ocupado...
        // Recordemos que sleep sale tras una interrupción, por lo que no debería
        // haber tiempo de espera entre que se producen las 10 interrupciones
        // y se termina el programa
        sleep(60); 
        printf("\nnint: %d, nstp: %d\n", nint, nstp);
    } 
    
    printf("\nnint: %d, nstp: %d\n", nint, nstp);

    return EXIT_SUCCESS;
}
