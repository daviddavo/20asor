#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include "daverror.h"

#define MAX_STR 4096

int main(int argc, char ** argv) {
    char * fname;
    char fnamesym[MAX_STR], fnamehard[MAX_STR];
    struct stat statbuf;

    if (argc > 1) fname = argv[1];
    else fname = "fichero";

    strcpy(fnamesym, fname);
    strcpy(fnamehard, fname);
    strncat(fnamesym, ".sym", MAX_STR-1);
    strncat(fnamehard, ".hard", MAX_STR-1);

    DAVO_CHECK(symlink(fname, fnamesym));
    DAVO_CHECK(link(fname, fnamehard));

    return 0;
}
