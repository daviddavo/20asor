#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "daverror.h"

int main(int argc, char ** argv) {
    char * fname;

    if (argc > 1) fname = argv[1];
    else fname = "fichero";

    DAVO_CHECK(open(fname, O_CREAT | O_WRONLY, 00645));

    return 0;
}
