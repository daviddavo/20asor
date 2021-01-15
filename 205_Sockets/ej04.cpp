#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netdb.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

#include "daverror.h"

/* Modificar el servidor para que, además de poder recibir comandos por red, 
 * los pueda recibir directamente por el terminal, leyendo dos caracteres (el
 * comando y ‘\n’) de la entrada estándar. Multiplexar el uso de ambos canales
 * usando select(2).
 */

#define BUF_SIZE 64

#define TIME_REPR "%X"
#define DATE_REPR "%x"

int main(int argc, char ** argv) {
    addrinfo hints {};
    addrinfo * res;
    sockaddr_storage caddr;
    socklen_t caddrlen;
    bool running = true, response;
    size_t size;
    fd_set set;
    time_t t;
    tm * tm;
    int ierror, sd, nfds;
    char * chptr;
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

    FD_ZERO(&set);

    printf("Server listening on %s:%s\n", bufhost, bufserv);
    while (running) {
        FD_SET(sd, &set);
        FD_SET(fileno(stdin), &set);

        DAVO_CHECK(nfds = select(sd+1, &set, NULL, NULL, NULL));

        for (int i = 0; i < nfds; i++) {
            // Read
            if (FD_ISSET(sd, &set)) {
                caddrlen = sizeof(caddr);
                DAVO_CHECK(size = recvfrom(sd, buf, BUF_SIZE, 0, (sockaddr*)&caddr, &caddrlen));
                getnameinfo((sockaddr *)&caddr, caddrlen, bufhost, BUF_SIZE, bufserv, BUF_SIZE, 0);
                printf("%ld bytes de [%s]:%s\n", size, bufhost, bufserv);
            } else if (FD_ISSET(fileno(stdin), &set)) {
                fgets(buf, BUF_SIZE, stdin);
            }

            // Evaluate
            t = time(NULL);
            tm = localtime(&t);
            response = false;

            switch (buf[0]) {
                case 't':
                    strftime(timebuf, BUF_SIZE, TIME_REPR, tm);
                    response = true;
                    break;
                case 'd':
                    strftime(timebuf, BUF_SIZE, DATE_REPR, tm);
                    response = true;
                    break;
                case 'q': running = false; break;
                case '\n': break;
                default: fprintf(stderr, "Comando no soportado: %c\n", buf[0]); break;
            }

            // Print
            if (FD_ISSET(sd, &set) && response) {
                DAVO_CHECK(sendto(sd, timebuf, strlen(timebuf), 0, (sockaddr*) &caddr, caddrlen));

                FD_CLR(sd, &set);
            } else if (FD_ISSET(fileno(stdin), &set) && response) {
                printf("%s\n", timebuf);

                FD_CLR(sd, &set);
            }
        }

    }

    DAVO_CHECK(close(sd));

    printf("Server stopped\n");
    freeaddrinfo(res);

    return EXIT_SUCCESS;
}
