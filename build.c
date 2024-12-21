#define NEXUS_IMPLEMENTATION
#include "nexus.h"

/* clang-format off */
#define COMMON_FLAGS \
    "-fdiagnostics-color=always", \
    "-fPIE", \
    "-ansi", \
    "-O2", \
    "-march=native", \
    "-flto"
/* clang-format on */

int main(int argc, char **argv) {
    NX_REBUILD(argc, argv);

    /* clang-format off */
    const char *glad_args[] = {
        "cc",
        "-c",
        "glad/glad.c",
        "-o",
        "glad.o"
    };

    const char *nexus_args[] = {
        "cc",
        "main.c",
        "glad.o",
        "-o",
        "nexus",
        "-lglfw",
        "-lm",
        COMMON_FLAGS
    };
    /* clang-format on */

    int glad_result = nx_compile_command("glad", glad_args, nx_len(glad_args), 0);
    if (glad_result != 0) {
        return glad_result;
    }

    int nexus_result = nx_compile_command("nexus", nexus_args, nx_len(nexus_args), 1);
    if (nexus_result != 0) {
        return nexus_result;
    }

    return EXIT_SUCCESS;
}
