#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

#include "daverror.h"

/* Crear otra tubería con nombre. Escribir un programa que espere hasta que 
 * haya datos listos para leer en alguna de ellas. El programa debe mostrar la
 * tubería desde la que leyó y los datos leídos. Consideraciones: 
 *   - Para optimizar las operaciones de lectura usar un buffer (ej. de 256 bytes).
 *   - Usar read(2) para leer de la tubería y gestionar adecuadamente la 
 *     longitud de los datos leídos.
 *   - Normalmente, la apertura de la tubería para lectura se bloqueará hasta
 *     que se abra para escritura (ej. con echo 1 > tuberia). Para evitarlo, 
 *     usar la opción O_NONBLOCK en open(2). 
 *   - Cuando el escritor termina y cierra la tubería, read(2) devolverá 0, 
 *     indicando el fin de fichero, por lo que hay que cerrar la tubería y
 *     volver a abrirla. Si no, select(2) considerará el descriptor siempre
 *     listo para lectura y no se bloqueará.
 */

#define BUF_SIZE 4096
#define N_FIFO 3

int main(int argc, char ** argv) {
    const char * const FIFO_PATHS[N_FIFO] = {
        "/tmp/test.fifo",
        "/tmp/test2.fifo",
        "/tmp/test3.fifo"
    };

    char buf[BUF_SIZE];
    size_t size;
    int fd[N_FIFO];
    int nfds;
    fd_set set;

    FD_ZERO(&set);

    // Abrimos las tuberías
    for (int i = 0; i < N_FIFO; i++) {
        DAVO_CHECK(nfds = fd[i] = open(FIFO_PATHS[i], O_RDONLY | O_NONBLOCK));
        FD_SET(fd[i], &set);
    }

    while (true) {
        DAVO_CHECK(select(nfds+1, &set, NULL, NULL, NULL));

        for (int i = 0; i < N_FIFO; i++) {
            if (FD_ISSET(fd[i], &set)) {
                size = read(fd[i], buf, BUF_SIZE);
                buf[size] = '\0';

                printf("Leido de %s:\n", FIFO_PATHS[i]);
                printf(buf);

                DAVO_CHECK(close(fd[i]));
                DAVO_CHECK(fd[i] = open(FIFO_PATHS[i], O_RDONLY | O_NONBLOCK));
                nfds = (fd[i] > nfds)?fd[i]:nfds;
            }

            FD_SET(fd[i], &set);
        }
    }
    

    for (int i = 0; i < N_FIFO; i++) {
        DAVO_CHECK(close(fd[i]));
    }


    return EXIT_SUCCESS;
}

