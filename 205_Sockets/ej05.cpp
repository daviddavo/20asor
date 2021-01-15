#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <sys/wait.h>

#include "daverror.h"

/* Convertir el servidor UDP en multi-proceso siguiendo el patrón pre-fork. 
 * Una vez asociado el socket a la dirección local con bind(2), crear varios
 * procesos que llamen a recvfrom(2) de forma que cada uno atenderá un mensaje
 * de forma concurrente. Imprimir el PID del proceso servidor para comprobarlo.
 */

#define BUF_SIZE 64

#define TIME_REPR "%X"
#define DATE_REPR "%x"

void server(int sd) {
    char buf[BUF_SIZE], timebuf[BUF_SIZE], bufhost[BUF_SIZE], bufserv[BUF_SIZE];
    sockaddr_storage caddr;
    socklen_t caddrlen;
    size_t size;
    time_t t;
    tm * tm;
    bool running = true;

    printf("Created thread pid: %d, gpid: %d\n", getpid(), getpgid(0));
    while (running) {
        caddrlen = sizeof(caddr);
        DAVO_CHECK(size = recvfrom(sd, buf, BUF_SIZE, 0, (sockaddr*)&caddr, &caddrlen));
        getnameinfo((sockaddr *)&caddr, caddrlen, bufhost, BUF_SIZE, bufserv, BUF_SIZE, 0);
        printf("%ld bytes de [%s]:%s at pid %d\n", size, bufhost, bufserv, getpid());

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

    exit(EXIT_SUCCESS);
}

int main(int argc, char ** argv) {
    addrinfo hints {};
    addrinfo * res;
    int ierror, sd, nproc;
    pid_t gpid = 0, pid = 0;
    char bufserv[BUF_SIZE], bufhost[BUF_SIZE];

    if (argc != 4) {
        fprintf(stderr, "Usage: %s host port processes\n", argv[0]);

        return EXIT_FAILURE;
    }

    nproc = atoi(argv[3]);
    hints.ai_socktype = SOCK_DGRAM;

    if ((ierror = getaddrinfo(argv[1], argv[2], &hints, &res))) {
        fprintf(stderr, "Error getaddrinfo(): %s for %s\n", gai_strerror(ierror), argv[1]);
    }
    
    DAVO_CHECK(getnameinfo(res->ai_addr, res->ai_addrlen, bufhost, BUF_SIZE, bufserv, BUF_SIZE, NI_NUMERICHOST | NI_NUMERICSERV));

    DAVO_CHECK(sd = socket(res->ai_family, res->ai_socktype, res->ai_protocol));
    DAVO_CHECK(bind(sd, res->ai_addr, res->ai_addrlen));

    printf("Server listening on %s:%s\n", bufhost, bufserv);

    // Invocamos el resto de hijos
    for (int i = 0; i < nproc; i++) {
        pid = DAVO_CHECK(fork());
        if (pid == 0) {

            DAVO_CHECK(setpgid(0, gpid));
            server(sd);
            exit(EXIT_FAILURE);
        } else if (i == 0) {
            gpid = pid;
        }
    }

    if (pid) {
        // Esperamos a que uno de los hijos acabe
        DAVO_CHECK(wait(&ierror));
        
        if (ierror != EXIT_SUCCESS) {
            fprintf(stderr, "One of childs failed\n");
        }

        printf("One of childs died\n");

        // Matamos el resto de procesos
        kill(-gpid, SIGTERM);

        for (int i = 0; i < nproc-1; i++) {
            DAVO_CHECK(wait(NULL));
        }

        DAVO_CHECK(close(sd));

        printf("Server stopped\n");
        freeaddrinfo(res);
    }

    return EXIT_SUCCESS;
}
