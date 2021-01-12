#include <unistd.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <stdio.h>
#include <string.h>

#include "daverror.h"

/* Un demonio es un proceso que se ejecuta en segundo plano para proporcionar 
 * un servicio. Normalmente, un demonio está en su propia sesión y grupo. Para
 * garantizar que es posible crear la sesión y el grupo, el demonio crea un
 * nuevo proceso para ejecutar la lógica del servicio y crear la nueva sesión.
 * Escribir una plantilla de demonio (creación del nuevo proceso y de la sesión
 * en el que únicamente se muestren los atributos del proceso (como en el
 * ejercicio anterior). Además, fijar el directorio de trabajo del demonio a
 * /tmp. */

void showvars() {
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
}

void hijo() {
    int ssid;

    DAVO_CHECK(chdir("/tmp"));
    DAVO_CHECK(ssid = setsid());

    printf("Soy el proceso hijo!\n");
    showvars();
    exit(EXIT_SUCCESS);
}

int main() {
    pid_t pid;

    DAVO_CHECK(pid = fork());
    if (pid == 0) {
        // sleep(10);
        hijo();
    } else {
        sleep(10);
        printf("Soy el proceso padre!\n");
        showvars();
    }


    return EXIT_SUCCESS;
}
