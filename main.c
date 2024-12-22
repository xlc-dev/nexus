#define NX_DEBUG

#define NXUI_IMPLEMENTATION
#include "nxui.h"

#define NEXUS_IMPLEMENTATION
#include "nexus.h"

#include <GLFW/glfw3.h>

static const char *test_vertex_src =
    "#version 330 core\n"
    "layout(location = 0) in vec3 aPos;\n"
    "uniform float testFloat;\n"
    "uniform int   testInt;\n"
    "uniform vec4  testVec4;\n"
    "void main()\n"
    "{\n"
    "    float offset = testFloat * 0.01 + float(testInt) * 0.001;\n"
    "    offset += testVec4.x * 0.0001;\n"
    "    gl_Position = vec4(aPos.x + offset, aPos.y, aPos.z, 1.0);\n"
    "}\n";

static const char *test_fragment_src =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "uniform float testFloat;\n"
    "uniform int   testInt;\n"
    "uniform vec4  testVec4;\n"
    "void main()\n"
    "{\n"
    "    float dummy = (testFloat * 0.0001) + float(testInt) * 0.0001;\n"
    "    FragColor = testVec4 + vec4(dummy, 0.0, 0.0, 0.0);\n"
    "}\n";

static float test_vertices[] = {
    0.0f, 0.5f, 0.0f, -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f,
};

static unsigned int test_indices[] = {0, 1, 2};

int main(void) {
    /*************************************************************************
     * 1) Nexus tests
     *************************************************************************/
    {
        /* nx_len: get the number of elements in an array */
        {
            int arr[] = {1, 2, 3, 4, 5};
            nx_assert(nx_len(arr) == 5, "nx_len failed for arr");
        }

        /* nx_end: get a pointer to the end of an array */
        {
            char *end;
            char  letters[] = {'a', 'b', 'c', 'd'};
            end             = nx_end(letters);
            nx_assert(*(end - 1) == 'd', "nx_end failed for letters");
        }

        /* nx_min and nx_max: find smaller/larger of two values */
        {
            nx_assert(nx_min(10, 20) == 10, "nx_min failed for 10,20");
            nx_assert(nx_max(10, 20) == 20, "nx_max failed for 10,20");
            nx_assert(nx_min(-10, -20) == -20, "nx_min failed for -10,-20");
            nx_assert(nx_max(-10, -20) == -10, "nx_max failed for -10,-20");
            nx_assert(nx_min(5, 5) == 5, "nx_min failed for equal values");
            nx_assert(nx_max(5, 5) == 5, "nx_max failed for equal values");
        }

        /* nx_abs: get the absolute value of an integer */
        {
            nx_assert(nx_abs(-15) == 15, "nx_abs failed for -15");
            nx_assert(nx_abs(15) == 15, "nx_abs failed for 15");
            nx_assert(nx_abs(0) == 0, "nx_abs failed for 0");
            nx_assert(nx_abs(-2147483648) == 2147483648u, "nx_abs failed for int_min");
        }

        /* nx_clamp: constrain a value to a range */
        {
            nx_assert(nx_clamp(5, 1, 10) == 5, "nx_clamp failed (within range)");
            nx_assert(nx_clamp(0, 1, 10) == 1, "nx_clamp failed (below range)");
            nx_assert(nx_clamp(15, 1, 10) == 10, "nx_clamp failed (above range)");
            nx_assert(nx_clamp(1, 1, 10) == 1, "nx_clamp failed (equal min)");
            nx_assert(nx_clamp(10, 1, 10) == 10, "nx_clamp failed (equal max)");
            nx_assert(nx_clamp(5, 5, 5) == 5, "nx_clamp failed (min=max)");
        }

        /* nx_swap: swap the values of two variables */
        {
            int    a, b, tmp;
            double tmp_d, x, y;

            a = 100;
            b = 200;
            nx_swap(a, b, tmp);
            nx_assert(a == 200 && b == 100, "nx_swap failed for int");

            x = 1.5;
            y = 2.5;
            nx_swap(x, y, tmp_d);
            nx_assert(x == 2.5 && y == 1.5, "nx_swap failed for double");
        }

        /* nx_swap for complex types: structs/pointers */
        {
            struct point {
                int x, y;
            } p1, p2, tmp_pnt;

            int  val1 = 10, val2 = 20;
            int *p1_ptr = &val1, *p2_ptr = &val2, *tmp_ptr;

            p1.x = 1;
            p1.y = 2;
            p2.x = 3;
            p2.y = 4;

            nx_swap(p1, p2, tmp_pnt);
            nx_assert(p1.x == 3 && p1.y == 4, "nx_swap failed (struct p1)");
            nx_assert(p2.x == 1 && p2.y == 2, "nx_swap failed (struct p2)");

            nx_swap(p1_ptr, p2_ptr, tmp_ptr);
            nx_assert(*p1_ptr == 20 && *p2_ptr == 10, "nx_swap failed (pointer)");
        }

        /* boolean operations and logic */
        {
            bool flag1 = true, flag2 = false;
            bool a = true, b = false;

            nx_assert(flag1 == true, "bool assignment failed for true");
            nx_assert(flag2 == false, "bool assignment failed for false");

            nx_assert((a && a) == true, "(true && true) failed");
            nx_assert((a && b) == false, "(true && false) failed");
            nx_assert((a || b) == true, "(true || false) failed");
            nx_assert((b || b) == false, "(false || false) failed");
        }

        /* Arena allocator tests */
        {
            void    *ptr1, *ptr2, *ptr3;
            NXArena *arena = nx_arena_create();
            nx_assert(arena != NULL, "arena_create failed");

            ptr1 = nx_arena_alloc(arena, 100);
            nx_assert(ptr1 != NULL, "arena_alloc failed for ptr1");

            ptr2 = nx_arena_alloc(arena, 200);
            nx_assert(ptr2 != NULL, "arena_alloc failed for ptr2");

            nx_arena_reset(arena);
            nx_assert(((NXArenaBlock *) arena->first)->used == 0, "arena_reset failed");

            ptr3 = nx_arena_alloc(arena, 50);
            nx_assert(ptr3 != NULL, "arena_alloc failed after reset");

            nx_arena_destroy(arena);
        }

        /* Single Linked List Tests */
        {
            NXSinglyLinkedList *list = nx_sll_create();
            int                 a = 1, b = 2, c = 3;

            nx_assert(list != NULL, "nx_sll_create failed");

            nx_sll_append(list, &a);
            nx_sll_append(list, &b);
            nx_sll_prepend(list, &c);

            nx_assert(*(int *) list->head->data == 3, "nx_sll_prepend failed");
            nx_assert(*(int *) list->head->next->data == 1, "nx_sll_append failed");
            nx_assert(*(int *) list->tail->data == 2, "nx_sll_append failed");

            nx_sll_remove(list, &b);
            nx_assert(*(int *) list->tail->data == 1, "nx_sll_remove failed");

            nx_sll_destroy(list);
        }

        /* Doubly Linked List Tests */
        {
            NXDoublyLinkedList *list = nx_dll_create();
            int                 a = 1, b = 2, c = 3;

            nx_assert(list != NULL, "nx_dll_create failed");

            nx_dll_append(list, &a);
            nx_dll_append(list, &b);
            nx_dll_prepend(list, &c);

            nx_assert(*(int *) list->head->data == 3, "nx_dll_prepend failed");
            nx_assert(*(int *) list->head->next->data == 1, "nx_dll_append failed");
            nx_assert(*(int *) list->tail->data == 2, "nx_dll_append failed");

            nx_dll_remove(list, &b);
            nx_assert(*(int *) list->tail->data == 1, "nx_dll_remove failed");

            nx_dll_destroy(list);
        }

        /* Hashmap Tests */
        {
            char *key1, *key2, *key3;
            int   value1 = 42, value2 = 33;
            int  *result;

            NXHashMap *map = nx_hashmap_create(NULL, NULL);
            nx_assert(map != NULL, "nx_hashmap_create failed");

            key1 = "apple";
            nx_assert(nx_hashmap_insert(map, key1, &value1), "insert failed for apple");

            key2 = "banana";
            nx_assert(nx_hashmap_insert(map, key2, &value2), "insert failed for banana");

            result = (int *) nx_hashmap_get(map, key1);
            nx_assert(result && *result == 42, "get failed for apple");

            result = (int *) nx_hashmap_get(map, key2);
            nx_assert(result && *result == 33, "get failed for banana");

            key3   = "cherry";
            result = (int *) nx_hashmap_get(map, key3);
            nx_assert(result == NULL, "get failed for non-existent cherry");

            nx_assert(nx_hashmap_remove(map, key1), "remove failed for apple");

            result = (int *) nx_hashmap_get(map, key1);
            nx_assert(result == NULL, "get after removal apple");

            nx_hashmap_destroy(map);
        }

        /* String Builder Tests */
        {
            NXStringBuilder *sb = nx_string_builder_create();
            nx_assert(sb != NULL, "Failed to create NXStringBuilder");

            nx_string_builder_append(sb, "Hello");
            {
                char msg[256];
                nx_snprintf(msg, sizeof(msg), "Expected 'Hello', got: %s",
                            nx_string_builder_to_cstring(sb));
                nx_assert(strcmp(nx_string_builder_to_cstring(sb), "Hello") == 0, msg);
            }

            nx_string_builder_append(sb, " ");
            nx_string_builder_append(sb, "World");
            {
                char msg[256];
                nx_snprintf(msg, sizeof(msg), "Expected 'Hello World', got: %s",
                            nx_string_builder_to_cstring(sb));
                nx_assert(strcmp(nx_string_builder_to_cstring(sb), "Hello World") == 0, msg);
            }

            nx_string_builder_append_char(sb, '!');
            {
                char msg[256];
                nx_snprintf(msg, sizeof(msg), "Expected 'Hello World!', got: %s",
                            nx_string_builder_to_cstring(sb));
                nx_assert(strcmp(nx_string_builder_to_cstring(sb), "Hello World!") == 0, msg);
            }

            nx_string_builder_clear(sb);
            {
                char msg[256];
                nx_snprintf(msg, sizeof(msg), "Expected empty string, got: %s",
                            nx_string_builder_to_cstring(sb));
                nx_assert(strcmp(nx_string_builder_to_cstring(sb), "") == 0, msg);
            }

            nx_string_builder_append(sb, "Nexus");
            {
                char msg[256];
                nx_snprintf(msg, sizeof(msg), "Expected 'Nexus', got: %s",
                            nx_string_builder_to_cstring(sb));
                nx_assert(strcmp(nx_string_builder_to_cstring(sb), "Nexus") == 0, msg);
            }

            nx_string_builder_destroy(sb);
        }

        /* FileIO Tests */
        {
            /* nx_file_open / nx_file_close */
            {
                const char *test_filename = "test_open.txt";
                const char *test_content  = "Hello, NXFile!";
                NXFile     *file;
                size_t      written;

                remove(test_filename);

                file = nx_file_open(test_filename, "w");
                nx_assert(file != NULL, "nx_file_open failed for writing");
                nx_assert(strcmp(file->filename, test_filename) == 0,
                          "nx_file_open filename mismatch");
                nx_assert(strcmp(file->mode, "w") == 0, "nx_file_open mode mismatch");

                written = fwrite(test_content, 1, strlen(test_content), file->file);
                nx_assert(written == strlen(test_content), "fwrite byte mismatch");

                nx_file_close(file);

                nx_assert(nx_file_exists(test_filename) == 1, "failed to create file");
                remove(test_filename);
            }

            /* nx_file_write_all, nx_file_read_all */
            {
                const char *test_filename = "test_write_read.txt";
                const char *test_content  = "Testing nx_file_write_all / nx_file_read_all.";
                char       *read_content;
                int         write_result;

                write_result = nx_file_write_all(test_filename, test_content);
                nx_assert(write_result == 0, "nx_file_write_all failed");

                nx_assert(nx_file_exists(test_filename) == 1, "file not created by write_all");

                read_content = nx_file_read_all(test_filename);
                nx_assert(read_content != NULL, "nx_file_read_all returned NULL");
                nx_assert(strcmp(read_content, test_content) == 0, "read_all mismatch");
                nx_free(read_content);

                remove(test_filename);
            }

            /* nx_file_exists */
            {
                const char *existing_file    = "existing_file.txt";
                const char *nonexistent_file = "nonexistent_file.txt";
                FILE       *file;

                file = fopen(existing_file, "w");
                nx_assert(file != NULL, "failed to create existing_file.txt");
                fclose(file);

                nx_assert(nx_file_exists(existing_file) == 1, "nx_file_exists false negative");
                nx_assert(nx_file_exists(nonexistent_file) == 0, "nx_file_exists false positive");

                remove(existing_file);
            }

            /* nx_file_size */
            {
                const char *test_filename  = "test_size.txt";
                const char *test_content   = "1234567890"; /* 10 bytes */
                const char *empty_filename = "empty_file.txt";
                long        size;
                int         write_result;
                FILE       *empty_file;

                write_result = nx_file_write_all(test_filename, test_content);
                nx_assert(write_result == 0, "nx_file_write_all failed for size test");

                size = nx_file_size(test_filename);
                nx_assert(size == 10, "nx_file_size mismatch (expected 10)");

                empty_file = fopen(empty_filename, "w");
                nx_assert(empty_file != NULL, "failed to create empty_file.txt");
                fclose(empty_file);

                size = nx_file_size(empty_filename);
                nx_assert(size == 0, "nx_file_size mismatch (expected 0)");

                size = nx_file_size("no_such_file.txt");
                nx_assert(size == -1, "nx_file_size mismatch (expected -1 for non-existent)");

                remove(test_filename);
                remove(empty_filename);
            }

            /* nx_file_read_all with binary data */
            {
                const char   *test_filename = "test_binary.bin";
                unsigned char binary_data[] = {0x00, 0xFF, 0x7E, 0x81, 0x55};
                size_t        binary_size   = sizeof(binary_data);
                FILE         *file;
                size_t        written;
                char         *read_data;

                file = fopen(test_filename, "wb");
                nx_assert(file != NULL, "failed to open test_binary.bin for writing");
                written = fwrite(binary_data, 1, binary_size, file);
                nx_assert(written == binary_size, "failed to write binary data");
                fclose(file);

                read_data = nx_file_read_all(test_filename);
                nx_assert(read_data != NULL, "nx_file_read_all failed for binary");
                nx_assert(memcmp(read_data, binary_data, binary_size) == 0, "binary data mismatch");
                nx_free(read_data);

                remove(test_filename);
            }

            /* nx_file_open with different modes */
            {
                const char *test_filename = "test_modes.txt";
                NXFile     *file_w, *file_a;
                char       *content;

                file_w = nx_file_open(test_filename, "w");
                nx_assert(file_w != NULL, "nx_file_open failed (mode=w)");
                fprintf(file_w->file, "Mode W Content\n");
                nx_file_close(file_w);

                file_a = nx_file_open(test_filename, "a");
                nx_assert(file_a != NULL, "nx_file_open failed (mode=a)");
                fprintf(file_a->file, "Mode A Content\n");
                nx_file_close(file_a);

                content = nx_file_read_all(test_filename);
                nx_assert(content != NULL, "nx_file_read_all multiple writes");
                nx_assert(strstr(content, "Mode W Content") != NULL, "missing 'Mode W Content'");
                nx_assert(strstr(content, "Mode A Content") != NULL, "missing 'Mode A Content'");
                nx_free(content);

                remove(test_filename);
            }

            /* nx_file_open invalid parameters */
            {
                NXFile *invalid_file, *nonexistent_file;

                invalid_file = nx_file_open("invalid_mode.txt", "invalid_mode");
                nx_assert(invalid_file == NULL, "nx_file_open should fail invalid mode");

                nonexistent_file = nx_file_open("nonexistent.txt", "r");
                nx_assert(nonexistent_file == NULL, "nx_file_open should fail non-existent(r)");
            }
        }
    }

    /*************************************************************************
     * 2) NXUI tests
     *************************************************************************/
    {
        GLFWwindow *window;

        if (!glfwInit()) {
            nx_die("Failed to initialize GLFW");
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(640, 480, "NXUI Test Window (Hidden)", NULL, NULL);
        if (!window) {
            glfwTerminate();
            nx_die("Failed to create hidden GLFW window");
        }
        glfwMakeContextCurrent(window);

        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            nx_die("Failed to initialize GLAD");
        }

        {
            NXUIContext *context = nxui_context_init();
            nx_assert(context != NULL, "nxui_context_init returned NULL");

            /* Add a shader that includes our declared uniforms. */
            {
                NXUIShaderProgram shader =
                    nxui_create_shader_program(test_vertex_src, test_fragment_src);
                nx_assert(shader.program_id != 0, "nxui_create_shader_program failed");

                nxui_context_add_shader(context, shader);
                nx_assert(context->shader_count == 1, "nxui_context_add_shader count != 1");
            }

            /* Create one mesh with a single attribute (position). */
            {
                NXUIMesh      mesh;
                NXUIAttribute attributes[1];
                attributes[0].index      = 0; /* matches 'layout(location = 0)' */
                attributes[0].size       = 3; /* 3 floats for position */
                attributes[0].type       = GL_FLOAT;
                attributes[0].normalized = GL_FALSE;
                attributes[0].stride     = 3 * sizeof(float);
                attributes[0].offset     = (void *) 0;

                mesh = nxui_create_mesh(test_vertices, sizeof(test_vertices), test_indices,
                                        sizeof(test_indices), 1, attributes, GL_STATIC_DRAW);
                nx_assert(mesh.vao != 0, "nxui_create_mesh failed (VAO=0)");
                nx_assert(mesh.vbo != 0, "nxui_create_mesh failed (VBO=0)");
                nx_assert(mesh.ebo != 0, "nxui_create_mesh failed (EBO=0)");
                nx_assert(mesh.index_count == 3, "nxui_create_mesh failed (idx_count!=3)");

                /* Assign the first (and only) shader in context to this mesh. */
                mesh.shader = &context->shaders[0];
                nxui_context_add_mesh(context, mesh);
                nx_assert(context->mesh_count == 1, "nxui_context_add_mesh count!=1");
            }

            /* Test uniform setting on the single available shader. */
            {
                nxui_use_shader_program(&context->shaders[0]);
                nxui_set_uniform_float(&context->shaders[0], "testFloat", 3.14f);
                nxui_set_uniform_int(&context->shaders[0], "testInt", 42);
                nxui_set_uniform_vec4(&context->shaders[0], "testVec4", 1.0f, 0.0f, 1.0f, 1.0f);
                /* nx_die() is called if the uniform is unused or missing in the shader. */
            }

            /* Attempt a quick single-pass render with our NXUI context. */
            {
                glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);

                nxui_render_ui(context);

                glfwSwapBuffers(window);
            }

            nxui_context_destroy(context);
        }

        glfwDestroyWindow(window);
        glfwTerminate();
    }

    nx_print_memory_leaks();
    printf("%sAll tests passed!%s\n", COLOR_GREEN, COLOR_RESET);

    return 0;
}
