# Nexus

Nexus is a collection of header-only [stb-style](https://github.com/nothings/stb) C89/ANSI C libraries designed to make working with C more bearable. It provides a central header (`nexus.h`) containing common data structures, macros, and utilities for everyday use.

## Headers

- [nexus.h](nexus.h): Contains essential data structures, macros, and utilities for general-purpose use. Making C bearable.
- [nxui.h](nxui.h): OpenGL low-level UI library.
- **glad/**: opengl core 3.3 loader.

## Installation

Since Nexus libraries are header-only, installation is easy. Just include `nexus.h` **and/or** any other specific header(s) in your project as needed.

## Examples

To see how Nexus works, take a look at `main.c`. This file contains tests that not only check that the functions in the `*.h` files work correctly but also show how to use them in practice. Each test demonstrates a feature of the library and serves as a usage example, making it easy to understand how to integrate Nexus into your own code.

## Contributing

If you would like to contribute, you need a C compiler and [GLFW](https://github.com/glfw/glfw) installed. To bootstrap the project, run the following commands:

```bash
git clone https://github.com/xlc-dev/nexus.git
cd nexus
cc -o build build.c
./build
```

build.c is our custom build system written in C, offering an alternative to traditional tools like `make` and `cmake`. Integrated within Nexus, we're dogfooding the nexus build system by using it in our own development process.

### Rules

Please follow these guidelines to ensure consistency across the codebase:

1. **Be C89/ANSI C Compliant**:
   - Nexus is written in C89. Avoid using features specific to C99 or later unless absolutely necessary.
   - While adding support for C++ is appreciated, it is **not a requirement**. If adding C++ support, ensure it does not break C compatibility.

2. **Clang Formatting**:
   - All code should follow the formatting style defined in the `.clang-format` file. You can use the `clang-format` CLI tool to format your file.

3. **Naming Conventions**:
   - All **functions and macros** should start with the `nx` prefix (e.g., `nx_glload_start()`, `nx_len()`, `nx_swap()`).
   - All **settings, header guards, and implementation-specific conditionals** should start with the `NX` prefix (e.g., `NX_HEADER_GUARD`, `NX_IMPLEMENTATION`).

4. **Feature-Specific Prefixes**:
   - When adding a new feature or set of related functions, use a descriptive prefix after `nx` or `NX` that clearly indicates the feature (e.g., `nx_gl_*` for OpenGL functions, `nx_sll_*` for singly linked list functions).

5. **Header-Only Structure**:
   - Each header file must follow a specific structure to be considered a single-header library. It should include both a **normal guard** to prevent multiple inclusions and an **implementation guard** to allow the file to be included as a single-header library.

   Example structure:
```c
   #ifndef NX_MY_HEADER_H
   #define NX_MY_HEADER_H

   /* Header contents go here... */

   #endif /* NX_MY_HEADER_H */

   #ifndef NX_MY_HEADER_IMPLEMENTATION
   #define NX_MY_HEADER_IMPLEMENTATION

   /* Implementation contents go here... */

   #endif /* NX_MY_HEADER_IMPLEMENTATION */
```

6. **Testing and Documentation**:
   - Every function (or feature) should be tested in the corresponding `*.c` files to ensure proper functionality.
   - Each header file should include a brief description at the top, outlining its purpose and usage. Additionally, include the license information at the top of the file.

By following these guidelines, you help maintain the consistency, portability, and usability of Nexus across different projects.

## License

Nexus is licensed under the [MPL-2.0 License](https://www.mozilla.org/en-US/MPL/2.0/). Please see the [LICENSE](LICENSE) file or the [MPL-2.0 FAQ](https://www.mozilla.org/en-US/MPL/2.0/FAQ/) for more details.
