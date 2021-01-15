#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

#include "daverror.h"

/* Escribir un servidor UDP de hora de forma que:
 * - La dirección y el puerto son el primer y segundo argumento del programa.
 *   Las direcciones pueden expresarse en cualquier formato (nombre de host, 
 *   notación de punto…). Además, el servidor debe funcionar con direcciones
 *   IPv4 e IPv6 .
 * - El servidor recibirá un comando (codificado en un carácter), de forma que
 *   ‘t’ devuelva la hora, ‘d’ devuelve la fecha y ‘q’ termina el proceso
 *   servidor.
 * - En cada mensaje el servidor debe imprimir el nombre y puerto del cliente,
 *   usar getnameinfo(3).
 * Probar el funcionamiento del servidor con la herramienta Netcat (comando nc
 * o ncat) como cliente.
 * Nota: Dado que el servidor puede funcionar con direcciones IPv4 e IPv6, 
 * hay que usar struct sockaddr_storage para acomodar cualquiera de ellas, por
 * ejemplo, en recvfrom(2).
 */

#define BUF_SIZE 64

#define TIME_REPR "%X"
#define DATE_REPR "%x"

int main(int argc, char ** argv) {
    addrinfo hints {};
    addrinfo * res;
    sockaddr_storage caddr;
    socklen_t caddrlen;
    bool running = true;
    size_t size, msgsize;
    time_t t;
    tm * tm;
    int ierror, sd;
    char buf[BUF_SIZE], bufserv[BUF_SIZE], bufhost[BUF_SIZE], timebuf[BUF_SIZE];

    if (argc != 3) {
        fprintf(stderr, "Usage: %s host port\n", argv[0]);

        return EXIT_FAILURE;
    }

    hints.ai_socktype = SOCK_DGRAM;

    if ((ierror = getaddrinfo(argv[1], argv[2], &hints, &res))) {
        fprintf(stderr, "Error getaddrinfo(): %s for %s\n", gai_strerror(ierror), argv[1]);
    }
    
    DAVO_CHECK(getnameinfo(res->ai_addr, res->ai_addrlen, bufhost, BUF_SIZE, bufserv, BUF_SIZE, NI_NUMERICHOST | NI_NUMERICSERV));

    DAVO_CHECK(sd = socket(res->ai_family, res->ai_socktype, res->ai_protocol));
    DAVO_CHECK(bind(sd, res->ai_addr, res->ai_addrlen));

    printf("Server listening on %s:%s\n", bufhost, bufserv);
    while (running) {
        caddrlen = sizeof(caddr);
        DAVO_CHECK(size = recvfrom(sd, buf, BUF_SIZE, 0, (sockaddr*)&caddr, &caddrlen));
        getnameinfo((sockaddr *)&caddr, caddrlen, bufhost, BUF_SIZE, bufserv, BUF_SIZE, 0);
        printf("%ld bytes de [%s]:%s\n", size, bufhost, bufserv);

        t = time(NULL);
        tm = localtime(&t);

        for (int i = 0; i < size; i++) {
            switch (buf[i]) {
                case 't':
                    strftime(timebuf, BUF_SIZE, TIME_REPR, tm);
                    DAVO_CHECK(sendto(sd, timebuf, strlen(timebuf), 0, (sockaddr*) &caddr, caddrlen));
                    break;
                case 'd':
                    strftime(timebuf, BUF_SIZE, DATE_REPR, tm);
                    DAVO_CHECK(sendto(sd, timebuf, strlen(timebuf), 0, (sockaddr*) &caddr, caddrlen));
                    break;
                case 'q': running = false; break;
                case '\n': break;
                default: fprintf(stderr, "Comando no soportado: %c\n", buf[i]); break;
            }
        }
    }

    DAVO_CHECK(close(sd));

    printf("Server stopped\n");
    freeaddrinfo(res);

    return EXIT_SUCCESS;
}
