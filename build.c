#define NEXUS_IMPLEMENTATION
#include "nexus.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    NX_REBUILD(argc, argv);

    NXCR *cr = nx_cr_create();
    if (!cr) {
        fprintf(stderr, "Failed to create NXCR instance.\n");
        return EXIT_FAILURE;
    }

    nx_cr_append(cr, "cc");
    nx_cr_append(cr, "nexus.c");
    nx_cr_append(cr, "-o");
    nx_cr_append(cr, "nexus");
    nx_cr_enable_gcc_warnings(cr);
    nx_cr_append(cr, "-fdiagnostics-color=always");
    nx_cr_append(cr, "-fPIE");
    nx_cr_append(cr, "-ansi");
    nx_cr_append(cr, "-O2");
    nx_cr_append(cr, "-march=native");
    nx_cr_append(cr, "-flto");

    int result = nx_cr_execute(cr);

    if (result == 0) {
        printf("%sCompilation successful!%s\n", COLOR_GREEN, COLOR_RESET);
    } else {
        printf("%sCompilation failed. Output:%s\n%s\n", COLOR_RED, COLOR_RESET,
               nx_cr_get_output(cr));
    }

    nx_cr_destroy(cr);
    return result;
}
