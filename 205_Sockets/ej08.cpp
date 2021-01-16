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

/* Modificar el código del servidor para que acepte varias conexiones
 * simultáneas. Cada petición debe gestionarse en un proceso diferente,
 * siguiendo el patrón accept-and-fork. El proceso padre debe cerrar el socket
 * devuelto por accept(2).
 */

#define BUF_SIZE 64
#define DEFAULT_BACKLOG 10

void handler(int signal) {
    pid_t pid;
    int status;
    
    // Pueden mandarnos la señal si mueren dos hijos a la vez
    // Pero hay que esperar a todos
    // waitpid retorna 0, no -1 si hay más hijos vivos
    while((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        if (status != EXIT_SUCCESS) {
            fprintf(stderr, "Warning: child with pid %d had problems %d\n", pid, status);
        }
    }
}

void tratarConexion(int csd, char * bufhost, char * bufserv) {
    size_t size;
    char buf[BUF_SIZE];

    printf("Conexión desde [%s]:%s en %d\n", bufhost, bufserv, getpid());
    while (true) {
        DAVO_CHECK(size = recv(csd, buf, BUF_SIZE, 0));
        if (size == 0) break; // Conexión terminada
        DAVO_CHECK(send(csd, buf, size, 0));
    }

    printf("Terminada conexión de [%s]:%s\n", bufhost, bufserv);
    DAVO_CHECK(close(csd));

    exit(EXIT_SUCCESS);
}

int main(int argc, char ** argv) {
    addrinfo hints {};
    addrinfo * res;
    sockaddr_storage caddr;
    socklen_t caddrlen;
    pid_t pid;
    int ierror, sd, csd;
    bool accepting = true;
    char bufserv[BUF_SIZE], bufhost[BUF_SIZE];

    if (argc != 3) {
        fprintf(stderr, "Usage: %s host port\n", argv[0]);

        return EXIT_FAILURE;
    }

    hints.ai_socktype = SOCK_STREAM; // TCP
    signal(SIGCHLD, handler);

    if ((ierror = getaddrinfo(argv[1], argv[2], &hints, &res))) {
        fprintf(stderr, "Error getaddrinfo(): %s for %s\n", gai_strerror(ierror), argv[1]);
    }
    
    DAVO_CHECK(getnameinfo(res->ai_addr, res->ai_addrlen, bufhost, BUF_SIZE, bufserv, BUF_SIZE, NI_NUMERICHOST | NI_NUMERICSERV));

    DAVO_CHECK(sd = socket(res->ai_family, res->ai_socktype, res->ai_protocol));
    DAVO_CHECK(bind(sd, res->ai_addr, res->ai_addrlen));
    DAVO_CHECK(listen(sd, DEFAULT_BACKLOG));

    printf("Server listening on %s:%s\n", bufhost, bufserv);
    while (accepting) {
        caddrlen = sizeof(caddr);
        DAVO_CHECK(csd = accept(sd, (sockaddr*)&caddr, &caddrlen));
        getnameinfo((sockaddr *)&caddr, caddrlen, bufhost, BUF_SIZE, bufserv, BUF_SIZE, 0);

        DAVO_CHECK(pid = fork());
        if (pid == 0) {
            tratarConexion(csd, bufhost, bufserv);
        }

    }

    DAVO_CHECK(close(sd));

    printf("Server stopped\n");
    freeaddrinfo(res);

    return EXIT_SUCCESS;
}
