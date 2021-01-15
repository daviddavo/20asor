#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

#include "daverror.h"

/* Escribir el cliente para el servidor de hora. El cliente recibirá como
 * argumentos la dirección del servidor, el puerto del servidor y el comando. 
 * Por ejemplo, para solicitar la hora, ./time_client 192.128.0.1 3000 t.
 */

#define BUF_SIZE 64

int main(int argc, char ** argv) {
    addrinfo hints {};
    addrinfo * res;
    size_t size;
    int ierror, sd;
    char buf[BUF_SIZE], bufserv[BUF_SIZE], bufhost[BUF_SIZE];

    if (argc != 4) {
        fprintf(stderr, "Usage: %s host port command\n", argv[0]);

        return EXIT_FAILURE;
    }

    if (strlen(argv[3]) != 1 || (argv[3][0] != 'd' && argv[3][0] != 't' && argv[3][0] != 'q')) {
        fprintf(stderr, "Avaliable commands are [d]ate, [t]time, [q]uit\n");
        return EXIT_FAILURE;
    }

    hints.ai_socktype = SOCK_DGRAM;

    if ((ierror = getaddrinfo(argv[1], argv[2], &hints, &res))) {
        fprintf(stderr, "Error getaddrinfo(): %s for %s\n", gai_strerror(ierror), argv[1]);
    }
    
    DAVO_CHECK(getnameinfo(res->ai_addr, res->ai_addrlen, bufhost, BUF_SIZE, bufserv, BUF_SIZE, NI_NUMERICHOST | NI_NUMERICSERV));

    DAVO_CHECK(sd = socket(res->ai_family, res->ai_socktype, res->ai_protocol));
    printf("Sending to %s:%s\n", bufhost, bufserv);

    DAVO_CHECK(sendto(sd, argv[3], strlen(argv[3]), 0, res->ai_addr, res->ai_addrlen));
    if (argv[3][0] != 'q') {
        DAVO_CHECK(size = recvfrom(sd, buf, BUF_SIZE, 0, NULL, NULL));
        printf("%s\n", buf);
    }

    DAVO_CHECK(close(sd));

    freeaddrinfo(res);

    return EXIT_SUCCESS;
}
