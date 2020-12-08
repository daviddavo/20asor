#include <unistd.h>
#include <fcntl.h>
#include "daverror.h"

int main(int argc, char ** argv) {
    int fd;

    if (argc < 2) return EXIT_FAILURE;

    printf("Opening file %s\n", argv[1]);
    DAVO_CHECK(fd = open(argv[1], O_WRONLY | O_CREAT, 00666));
    DAVO_CHECK(dup2(fd, STDOUT_FILENO));

    printf("Hello World from fd %d\n", fd);
    fprintf(stdout, "This is another test\n");
    fprintf(stderr, "This should appear in the terminal\n");

    return EXIT_SUCCESS;
}
