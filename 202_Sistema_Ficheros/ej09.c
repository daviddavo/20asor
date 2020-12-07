#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <fcntl.h>
#include <time.h>
#include "daverror.h"

char * translateType(long unsigned type) {
    if (S_ISSOCK(type)) return "socket";
    else if (S_ISLNK(type)) return "symbolic link";
    else if (S_ISREG(type)) return "regular file";
    else if (S_ISBLK(type)) return "block device";
    else if (S_ISDIR(type)) return "directory";
    else if (S_ISCHR(type)) return "character device";
    else if (S_ISFIFO(type)) return "FIFO";
    else return "Error?";
}

int main(int argc, char ** argv) {
    char * fname;
    struct stat statbuf;

    if (argc > 1) fname = argv[1];
    else fname = "fichero";

    DAVO_CHECK(stat(fname, &statbuf));

    printf("Device: %d:%02d\n", major(statbuf.st_dev), minor(statbuf.st_dev));
    printf(" Inode: %lu\n", statbuf.st_ino);
    printf("  Type: %s\n", translateType(statbuf.st_mode));
    printf("Access: %s\n", ctime(&statbuf.st_atime));

    // Nota:
    // st_mtime es el tiempo de modificación de los DATOS
    // st_ctime es la ultima vez que cambió el inodo (permisos, etc.)

    return 0;
}
