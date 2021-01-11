#include <unistd.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <stdio.h>
#include <string.h>

#include "daverror.h"

// VERSIÓN SYSTEM

/* Escribir dos versiones, una con system(3) y otra con execvp(3), de un
 * programa que ejecute otro programa que se pasará como argumento por línea de
 * comandos. En cada caso, se debe imprimir la cadena “El comando terminó de
 * ejecutarse” después de la ejecución. ¿En qué casos se imprime la cadena? 
 * ¿Por qué? 
 * No se imprimirá con exec, pues no hace fork, sustituye el código del programa
 */

int main(int argc, char ** argv) {
    int ex;
    char buf[4096];

    if (argc < 2) {
        fprintf(stderr, "Introduce un comando\n");
        return EXIT_FAILURE;
    }

    // Como system recibe solo una string, tenemos que convertir los args
    strcpy(buf, argv[1]);
    for (int i = 2; i < argc; i++) {
        strcat(buf, " ");
        strcat(buf, argv[i]);
    }


    ex = system(buf);
    // Código inaccesible
    printf("El comando terminó de ejecutarse\n");

    return ex;
}
