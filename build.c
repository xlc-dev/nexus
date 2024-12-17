#define NEXUS_IMPLEMENTATION
#include "nexus.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    NX_REBUILD(argc, argv);

    NXCLI *cli = nx_cli_create();
    if (!cli) {
        fprintf(stderr, "Failed to create NXCLI instance.\n");
        return EXIT_FAILURE;
    }

    nx_cli_append(cli, "cc");
    nx_cli_append(cli, "nexus.c");
    nx_cli_append(cli, "-o");
    nx_cli_append(cli, "nexus");
    nx_cli_enable_gcc_warnings(cli);
    nx_cli_append(cli, "-fdiagnostics-color=always");
    nx_cli_append(cli, "-fPIE");
    nx_cli_append(cli, "-ansi");
    nx_cli_append(cli, "-O2");
    nx_cli_append(cli, "-march=native");
    nx_cli_append(cli, "-flto");

    int result = nx_cli_execute(cli);

    if (result == 0) {
        printf("%sCompilation successful!%s\n", COLOR_GREEN, COLOR_RESET);
    } else {
        printf("%sCompilation failed. Output:%s\n%s\n", COLOR_RED, COLOR_RESET,
               nx_cli_get_output(cli));
    }

    nx_cli_destroy(cli);
    return result;
}
