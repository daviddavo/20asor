#include <unistd.h>
#include <stdlib.h>
#include <sched.h>
#include <stdio.h>
#include <string.h>

#include "daverror.h"

/* Escribir un programa que muestre la política de planificación (como cadena) 
 * y la prioridad del proceso actual, además de mostrar los valores máximo y
 * mínimo de la prioridad para la política de planificación.
 */

const char* sched2string(int sch) {
    switch (sch) {
        case SCHED_FIFO:
            return "First in-first out";
        case SCHED_RR:
            return "Round Robin";
        case SCHED_OTHER:
            return "Other (Linux Default)";
        default:
            return "Error";
    }
}

int main() {
    int sch, schmin, schmax;
    sched_param schpar;
    const char * schstr;

    DAVO_CHECK(sch = sched_getscheduler(0));
    schstr = sched2string(sch);
    DAVO_CHECK(sched_getparam(0, &schpar));
    DAVO_CHECK(schmin = sched_get_priority_min(sch));
    DAVO_CHECK(schmax = sched_get_priority_max(sch));

    printf("Scheduler: %s\n", schstr);
    printf("Priority:  %d\n", schpar.sched_priority);
    printf("min/max: %d/%d\n", schmin, schmax);

    return EXIT_SUCCESS;
}
