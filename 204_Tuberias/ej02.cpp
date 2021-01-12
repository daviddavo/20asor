#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "daverror.h"

/* Para la comunicación bi-direccional, es necesario crear dos tuberías, una
 * para cada sentido: p_h y h_p. Escribir un programa que implemente el
 * mecanismo de sincronización de parada y espera: 
 *   - El padre leerá de la entrada estándar (terminal) y enviará el mensaje al
 *     proceso hijo, escribiéndolo en la tubería p_h. Entonces permanecerá
 *     bloqueado esperando la confirmación por parte del hijo en la otra 
 *     tubería, h_p.
 *   - El hijo leerá de la tubería p_h, escribirá el mensaje por la salida
 *     estándar y esperará 1 segundo. Entonces, enviará el carácter ‘l’ al
 *     proceso padre, escribiéndolo en la tubería h_p, para indicar que está
 *     listo. Después de 10 mensajes enviará el carácter ‘q’ para indicar al
 *     padre que finalice.
 */

#define READER 0
#define WRITER 1
#define BUF_SIZE 1024

int main(int argc, char ** argv) {
    size_t size;
    char msg;
    char buf[BUF_SIZE];
    char * line;
    int nmsgs = 0;
    int p_h[2];
    int h_p[2];
    pid_t pid;

    DAVO_CHECK(pipe(h_p));
    DAVO_CHECK(pipe(p_h));
    DAVO_CHECK(pid = fork());

    if (pid) { // Padre escritor
        // Cerramos fd innecesarios
        DAVO_CHECK(close(p_h[READER]));
        DAVO_CHECK(close(h_p[WRITER]));

        // El padre lee de la salida estandar y se lo manda al hijo
        // (Entiendo que significa que no podemos redir entrada a pipe)
        do {
            DAVO_CHECK(getline(&line, &size, stdin));
            DAVO_CHECK(write(p_h[WRITER], line, size));

            free(line);
            line = NULL;
            size = 0;

            DAVO_CHECK(read(h_p[READER], &msg, 1));
            fprintf(stderr, "Recibido: %c\n", msg);
        } while (msg != 'q');

        DAVO_CHECK(close(p_h[WRITER]));
        DAVO_CHECK(close(h_p[READER]));
    } else { // Hijo
        // Cerramos fd innecesarios
        DAVO_CHECK(close(p_h[WRITER]));
        DAVO_CHECK(close(h_p[READER]));

        do {
            DAVO_CHECK(size = read(p_h[READER], &buf, BUF_SIZE));
            DAVO_CHECK(fputs(buf, stdout));

            DAVO_CHECK(write(h_p[WRITER], (nmsgs < 9)?"l":"q", 1));
            nmsgs++;
        } while (nmsgs < 10);

        printf("Terminé\n");
    }

    return EXIT_SUCCESS;
}

