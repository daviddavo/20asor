#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

#include "daverror.h"

/* Crear un servidor TCP de eco que escuche por conexiones entrantes en una 
 * dirección (IPv4 o IPv6) y puerto dados. Cuando reciba una conexión entrante,
 * debe mostrar la dirección y número de puerto del cliente. A partir de ese
 * momento, enviará al cliente todo lo que reciba desde el mismo (eco).
 * Comprobar su funcionamiento empleando la herramienta Netcat como cliente.
 * Comprobar qué sucede si varios clientes intentan conectar al mismo tiempo.
 */

#define BUF_SIZE 64
#define DEFAULT_BACKLOG 10

int main(int argc, char ** argv) {
    addrinfo hints {};
    addrinfo * res;
    sockaddr_storage caddr;
    socklen_t caddrlen;
    bool running = true;
    size_t size;
    int ierror, sd, csd;
    char buf[BUF_SIZE], bufserv[BUF_SIZE], bufhost[BUF_SIZE];

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
    DAVO_CHECK(bind(sd, res->ai_addr, res->ai_addrlen));
    DAVO_CHECK(listen(sd, DEFAULT_BACKLOG));

    printf("Server listening on %s:%s\n", bufhost, bufserv);
    while (running) {
        caddrlen = sizeof(caddr);
        // DAVO_CHECK(size = recvfrom(sd, buf, BUF_SIZE, 0, (sockaddr*)&caddr, &caddrlen));
        DAVO_CHECK(csd = accept(sd, (sockaddr*)&caddr, &caddrlen));
        getnameinfo((sockaddr *)&caddr, caddrlen, bufhost, BUF_SIZE, bufserv, BUF_SIZE, 0);
        printf("Conexión desde [%s]:%s\n", bufhost, bufserv);
        while (true) {
            DAVO_CHECK(size = recv(csd, buf, BUF_SIZE, 0));
            if (size == 0) break; // Conexión terminada
            DAVO_CHECK(send(csd, buf, size, 0));
        }

        printf("Terminada conexión de [%s]:%s\n", bufhost, bufserv);
        DAVO_CHECK(close(csd));
    }

    DAVO_CHECK(close(sd));

    printf("Server stopped\n");
    freeaddrinfo(res);

    return EXIT_SUCCESS;
}
