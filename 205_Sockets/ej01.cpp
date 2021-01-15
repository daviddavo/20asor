#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/types.h>

#include "daverror.h"

/* Escribir un programa que obtenga todas las posibles direcciones con las que
 * se podría crear un socket asociado a un host dado como primer argumento del
 * programa. Para cada dirección, mostrar la IP numérica, la familia de
 * protocolos y tipo de socket. Comprobar el resultado para:
 *   - Una dirección IPv4 válida (ej. “147.96.1.9”).
 *   - Una dirección IPv6 válida (ej. “fd00::a:0:0:0:1”).
 *   - Un nombre de dominio válido (ej. “www.google.com”).
 *   - Un nombre en /etc/hosts válido (ej. “localhost”).
 *   - Una dirección o nombre incorrectos en cualquiera de los casos anteriores.
 */

#define BUF_SIZE 64

const char * p2str(int ai_family) {
    switch (ai_family) {
        case AF_INET: return "IPv4";
        case AF_INET6: return "IPv6";
        case AF_UNSPEC: return "Both";
        default: return "??";
    }
}

const char * s2str(int ai_socktype) {
    switch (ai_socktype) {
        case SOCK_STREAM: return "Stream";
        case SOCK_DGRAM: return "Dgram";
        case SOCK_RAW: return "Raw";
        default: return "??";
    }
}

int main(int argc, char ** argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s addr\n", argv[0]);
        return EXIT_FAILURE;
    }

    char buf[BUF_SIZE];
    int ierror;
    addrinfo hints {};
    addrinfo * res;

    for (int i = 1; i < argc; i++) {
        if ((ierror = getaddrinfo(argv[i], NULL, &hints, &res))) {
            fprintf(stderr, "Error getaddrinfo(): %s for %s\n", gai_strerror(ierror), argv[i]);
            continue;
        }

        for (; res->ai_next; res = res->ai_next) {
            printf("%s => %s - ", argv[i], p2str(res->ai_family));
            
            DAVO_CHECK(getnameinfo(res->ai_addr, res->ai_addrlen, buf, BUF_SIZE, NULL, 0, NI_NUMERICHOST));

            printf("%s (%s)\n", buf, s2str(res->ai_socktype));
        }

        freeaddrinfo(res);
    }

    return EXIT_SUCCESS;
}
