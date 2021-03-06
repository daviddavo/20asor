#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "daverror.h"

/* Escribir un programa que emule el comportamiento de la shell en la
 * ejecución de una sentencia en la forma: 
 *     comando1 argumento1 | comando2 argumento2
 * El programa creará una tubería sin nombre y creará un hijo: 
 *   - El proceso padre redireccionará la salida estándar al extremo de
 *        escritura de la tubería y ejecutará comando1 argumento1. 
 *   - El proceso hijo redireccionará la entrada estándar al extremo de
 *        lectura de la tubería y ejecutará comando2 argumento2.
 * Probar el funcionamiento con una sentencia similar a: ./ejercicio1 echo 12345 wc -c
 */

#define READER 0
#define WRITER 1

int main(int argc, char ** argv) {
    int pipefd[2];
    pid_t pid;

    if (argc != 5) {
        printf("Usage: %s comando1 argumento1 comando2 argumento2\n", argv[0]);
    }

    DAVO_CHECK(pipe(pipefd));
    DAVO_CHECK(pid = fork());

    if (pid) { // Padre escritor
        DAVO_CHECK(close(pipefd[READER]));
        DAVO_CHECK(dup2(pipefd[WRITER], fileno(stdout)));
        DAVO_CHECK(close(pipefd[WRITER]));

        DAVO_CHECK(execlp(argv[1], argv[1], argv[2], NULL));
    } else { // Hijo
        DAVO_CHECK(close(pipefd[WRITER]));
        DAVO_CHECK(dup2(pipefd[READER], fileno(stdin)));
        DAVO_CHECK(close(pipefd[READER]));

        DAVO_CHECK(execlp(argv[3], argv[1], argv[4], NULL));
    }

    return EXIT_SUCCESS;
}

