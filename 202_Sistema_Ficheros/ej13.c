#include <unistd.h>
#include <fcntl.h>
#include "daverror.h"

int main(int argc, char ** argv) {
    int fd;

    if (argc < 2) return EXIT_FAILURE;

    printf("Opening file %s\n", argv[1]);
    DAVO_CHECK(fd = open(argv[1], O_WRONLY | O_CREAT, 00666));
    DAVO_CHECK(dup2(fd, STDOUT_FILENO));
    DAVO_CHECK(dup2(STDOUT_FILENO, STDERR_FILENO));
    // Equivalente a dup2(fd, STDERR_FILENO)
    // Nota: De poner la redireccion de stderr > stdout antes que la de stdout > file
    //       escribiríamos la stderr en stdout y no en file

    printf("Hello World from fd %d\n", fd);
    fprintf(stdout, "This is another test\n");
    fprintf(stderr, "This should not appear in the terminal\n");

    return EXIT_SUCCESS;
}
