#include <unistd.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <stdio.h>
#include <string.h>

#include "daverror.h"

/* Escribir un programa que muestre los identificadores del proceso: 
 * identificador de proceso, de proceso padre, de grupo de procesos y de sesión.
 * Mostrar además el número máximo de archivos que puede abrir el proceso y el 
 * directorio de trabajo actual. */

int main() {
    int pid, ppid, pgid, sid;
    rlimit nofile;
    char * cwd;

    pid = getpid();
    ppid = getppid();
    pgid = getpgid(pid);
    sid = getsid(pid);
    cwd = get_current_dir_name();
    DAVO_CHECK(getrlimit(RLIMIT_NOFILE, &nofile));

    printf("pid: %d\n", pid);
    printf("ppid: %d\n", ppid);
    printf("pgid: %d\n", pgid);
    printf("psid: %d\n", sid);
    printf("maxfd: %ld\n", nofile.rlim_cur);
    printf("cwd: %s\n", cwd);

    return EXIT_SUCCESS;
}
