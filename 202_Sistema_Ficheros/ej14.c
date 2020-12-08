#include <unistd.h>
#include <fcntl.h>
#include "daverror.h"

int main(int argc, char ** argv) {
    int fd;

    if (argc < 2) return EXIT_FAILURE;

    printf("Opening file %s\n", argv[1]);
    DAVO_CHECK(fd = open(argv[1], O_WRONLY | O_CREAT, 00666));

    // DAVO_CHECK(dup2(fd, STDOUT_FILENO));
    DAVO_CHECK(close(STDOUT_FILENO));
    DAVO_CHECK(fcntl(fd, F_DUPFD, STDOUT_FILENO));

    // DAVO_CHECK(dup2(STDOUT_FILENO, STDERR_FILENO));
    DAVO_CHECK(close(STDERR_FILENO));
    DAVO_CHECK(fcntl(STDOUT_FILENO, F_DUPFD, STDERR_FILENO));

    // Nota: La diferencia es que dup2 hace las dos operaciones atómicamente y
    // por lo tanto es thread-safe

    printf("Hello World from fd %d\n", fd);
    fprintf(stdout, "This is another test\n");
    fprintf(stderr, "This should not appear in the terminal\n");

    return EXIT_SUCCESS;
}
