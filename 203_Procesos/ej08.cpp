#include <unistd.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

#include "daverror.h"

int main(int argc, char ** argv) {
    pid_t pid;
    int fdi, fdo, fde;

    if (argc < 2) {
        fprintf(stderr, "Introduce un comando\n");
        return EXIT_FAILURE;
    }
    
    DAVO_CHECK(pid = fork());
    if (pid == 0) {
        DAVO_CHECK(chdir("/tmp"));
        
        DAVO_CHECK(fdi = open("/dev/null", O_RDONLY, 0777));
        DAVO_CHECK(fdo = open("daemon.out", O_WRONLY | O_CREAT | O_TRUNC , 0777));
        DAVO_CHECK(fde = open("daemon.err", O_WRONLY | O_CREAT | O_TRUNC, 0777));

        DAVO_CHECK(dup2(fdi, fileno(stdin)));
        DAVO_CHECK(dup2(fdo, fileno(stdout)));
        DAVO_CHECK(dup2(fde, fileno(stderr)));

        DAVO_CHECK(execvp(argv[1], &argv[1]));
    } else {
        printf("Running %s in the background\n", argv[1]);
    }

    return EXIT_SUCCESS;
}
