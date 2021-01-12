#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

#include "daverror.h"

/* Escribir un programa que abra la tubería con el nombre anterior en modo sólo
 * escritura, y escriba en ella el primer argumento del programa. En otro
 * terminal, leer de la tubería usando un comando adecuado.
 */

#define FIFO_PATH "/tmp/test.fifo"

int main(int argc, char ** argv) {
    char buf[4096];
    int fd;

    if (argc < 2) {
        fprintf(stderr, "Introduce un texto\n");
        return EXIT_FAILURE;
    }

    strcpy(buf, argv[1]);
    for (int i = 2; i < argc; i++) {
        strcat(buf, " ");
        strcat(buf, argv[i]);
    }
    strcat(buf, "\n");

    DAVO_CHECK(fd = open(FIFO_PATH, O_WRONLY));

    DAVO_CHECK(write(fd, buf, strlen(buf)));

    DAVO_CHECK(close(fd));

    return EXIT_SUCCESS;
}

