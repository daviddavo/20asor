#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

#include "daverror.h"

/* Escribir el cliente para conectarse con el servidor del ejercicio anterior.
 * El cliente recibirá la dirección y el puerto del servidor como argumentos y,
 * una vez establecida la conexión con el servidor, le enviará lo que el
 * usuario escriba por teclado. Mostrará en la consola la respuesta recibida 
 * desde el servidor. Cuando el usuario escriba el carácter ‘Q’ como único
 * carácter de una línea, el cliente cerrará la conexión con el servidor.
 */

#define BUF_SIZE 64

int main(int argc, char ** argv) {
    addrinfo hints {};
    addrinfo * res;
    bool running = true;
    size_t size;
    int ierror, sd;
    char bufserv[BUF_SIZE], bufhost[BUF_SIZE];
    char * line;

    if (argc != 3) {
        fprintf(stderr, "Usage: %s host port\n", argv[0]);

        return EXIT_FAILURE;
    }

    hints.ai_socktype = SOCK_STREAM; // TCP

    if ((ierror = getaddrinfo(argv[1], argv[2], &hints, &res))) {
        fprintf(stderr, "Error getaddrinfo(): %s for %s\n", gai_strerror(ierror), argv[1]);
    }
    
    DAVO_CHECK(getnameinfo(res->ai_addr, res->ai_addrlen, bufhost, BUF_SIZE, bufserv, BUF_SIZE, NI_NUMERICHOST | NI_NUMERICSERV));

    DAVO_CHECK(sd = socket(res->ai_family, res->ai_socktype, res->ai_protocol));
    DAVO_CHECK(connect(sd, res->ai_addr, res->ai_addrlen));

    printf("Connected to [%s]:%s\n", bufhost, bufserv);
    while (running) {
        size = 0;
        line = NULL;
        DAVO_CHECK(size = getline(&line, &size, stdin));

        if (size == 2 && line[0] == 'Q') {
            running = false;
        } else {
            DAVO_CHECK(send(sd, line, size, 0));
            DAVO_CHECK(recv(sd, line, size, 0));
            fputs(line, stdout);
        }

        free(line);
    }

    DAVO_CHECK(close(sd));

    printf("Client stopped\n");
    freeaddrinfo(res);

    return EXIT_SUCCESS;
}
