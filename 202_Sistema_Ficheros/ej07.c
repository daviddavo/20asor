#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "daverror.h"

int main(int argc, char ** argv) {
    char * fname;

    if (argc > 1) fname = argv[1];
    else fname = "fichero";

    DAVO_CHECK(umask(~00645));
    
    // Podemos usar también creat()
    DAVO_CHECK(open(fname, O_CREAT | O_WRONLY, 00777));

    return 0;
}
