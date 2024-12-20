#define NX_DEBUG
#define NEXUS_IMPLEMENTATION
#include "nexus.h"

int main(void) {
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

    /* nx_min and nx_max: find the smaller or larger of two values */
    {
        nx_assert(nx_min(10, 20) == 10, "nx_min failed for 10 and 20");
        nx_assert(nx_max(10, 20) == 20, "nx_max failed for 10 and 20");
        nx_assert(nx_min(-10, -20) == -20, "nx_min failed for -10 and -20");
        nx_assert(nx_max(-10, -20) == -10, "nx_max failed for -10 and -20");
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
        nx_assert(nx_clamp(5, 1, 10) == 5, "nx_clamp failed for value within range");
        nx_assert(nx_clamp(0, 1, 10) == 1, "nx_clamp failed for value below range");
        nx_assert(nx_clamp(15, 1, 10) == 10, "nx_clamp failed for value above range");
        nx_assert(nx_clamp(1, 1, 10) == 1, "nx_clamp failed for value equal to min");
        nx_assert(nx_clamp(10, 1, 10) == 10, "nx_clamp failed for value equal to max");
        nx_assert(nx_clamp(5, 5, 5) == 5, "nx_clamp failed for min equal to max");
    }

    /* nx_swap: swap the values of two variables */
    {
        int    a, b, tmp;
        double tmp_d, x, y;

        a = 100;
        b = 200;
        nx_swap(a, b, tmp);
        nx_assert(a == 200 && b == 100, "nx_swap failed for integers");

        x = 1.5;
        y = 2.5;
        nx_swap(x, y, tmp_d);
        nx_assert(x == 2.5 && y == 1.5, "nx_swap failed for doubles");
    }

    /* nx_swap for complex types: structs and pointers */
    {
        struct point {
            int x, y;
        } p1, p2, tmp_pnt;

        int  val1, val2;
        int *p1_ptr, *p2_ptr, *tmp_ptr;

        p1.x = 1;
        p1.y = 2;
        p2.x = 3;
        p2.y = 4;

        val1   = 10;
        val2   = 20;
        p1_ptr = &val1;
        p2_ptr = &val2;

        nx_swap(p1, p2, tmp_pnt);
        nx_assert(p1.x == 3 && p1.y == 4, "nx_swap failed for structs");
        nx_assert(p2.x == 1 && p2.y == 2, "nx_swap failed for structs");

        nx_swap(p1_ptr, p2_ptr, tmp_ptr);
        nx_assert(*p1_ptr == 20 && *p2_ptr == 10, "nx_swap failed for pointers");
    }

    /* boolean operations and logic */
    {
        bool flag1, flag2, a, b;

        flag1 = true;
        flag2 = false;
        a     = true;
        b     = false;

        nx_assert(flag1 == true, "bool assignment failed for true");
        nx_assert(flag2 == false, "bool assignment failed for false");

        nx_assert((a && a) == true, "bool logic and failed for (true && true)");
        nx_assert((a && b) == false, "bool logic and failed for (true && false)");
        nx_assert((a || b) == true, "bool logic or failed for (true || false)");
        nx_assert((b || b) == false, "bool logic or failed for (false || false)");
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
        int                 a, b, c;

        nx_assert(list != NULL, "nx_sll_create failed");

        a = 1;
        b = 2;
        c = 3;
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
        int                 a, b, c;

        nx_assert(list != NULL, "nx_dll_create failed");

        a = 1;
        b = 2;
        c = 3;
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
        nx_assert(nx_hashmap_insert(map, key1, &value1), "nx_hashmap_insert failed for 'apple'");

        key2 = "banana";
        nx_assert(nx_hashmap_insert(map, key2, &value2), "nx_hashmap_insert failed for 'banana'");

        result = (int *) nx_hashmap_get(map, key1);
        nx_assert(result != NULL && *result == 42, "nx_hashmap_get failed for 'apple'");

        result = (int *) nx_hashmap_get(map, key2);
        nx_assert(result != NULL && *result == 33, "nx_hashmap_get failed for 'banana'");

        key3   = "cherry";
        result = (int *) nx_hashmap_get(map, key3);
        nx_assert(result == NULL, "nx_hashmap_get failed for non-existent key 'cherry'");

        nx_assert(nx_hashmap_remove(map, key1), "nx_hashmap_remove failed for 'apple'");

        result = (int *) nx_hashmap_get(map, key1);
        nx_assert(result == NULL, "nx_hashmap_get failed after removal of 'apple'");

        nx_hashmap_destroy(map);
    }

    /* String Builder Tests */
    {
        NXStringBuilder *sb = nx_string_builder_create();
        nx_assert(sb != NULL, "Failed to create NXStringBuilder");

        nx_string_builder_append(sb, "Hello");
        {
            char msg[256];
            nx_snprintf(msg, sizeof(msg), "Expected 'Hello', but got: %s",
                        nx_string_builder_to_cstring(sb));
            nx_assert(strcmp(nx_string_builder_to_cstring(sb), "Hello") == 0, msg);
        }

        nx_string_builder_append(sb, " ");
        nx_string_builder_append(sb, "World");
        {
            char msg[256];
            nx_snprintf(msg, sizeof(msg), "Expected 'Hello World', but got: %s",
                        nx_string_builder_to_cstring(sb));
            nx_assert(strcmp(nx_string_builder_to_cstring(sb), "Hello World") == 0, msg);
        }

        nx_string_builder_append_char(sb, '!');
        {
            char msg[256];
            nx_snprintf(msg, sizeof(msg), "Expected 'Hello World!', but got: %s",
                        nx_string_builder_to_cstring(sb));
            nx_assert(strcmp(nx_string_builder_to_cstring(sb), "Hello World!") == 0, msg);
        }

        nx_string_builder_clear(sb);
        {
            char msg[256];
            nx_snprintf(msg, sizeof(msg), "Expected empty string, but got: %s",
                        nx_string_builder_to_cstring(sb));
            nx_assert(strcmp(nx_string_builder_to_cstring(sb), "") == 0, msg);
        }

        nx_string_builder_append(sb, "Nexus");
        {
            char msg[256];
            nx_snprintf(msg, sizeof(msg), "Expected 'Nexus', but got: %s",
                        nx_string_builder_to_cstring(sb));
            nx_assert(strcmp(nx_string_builder_to_cstring(sb), "Nexus") == 0, msg);
        }

        nx_string_builder_destroy(sb);
    }
    /* FileIO Tests */
    {
        /* Test nx_file_open and nx_file_close */
        {
            const char *test_filename = "test_open.txt";
            const char *test_content  = "Hello, NXFile!";
            NXFile     *file;
            size_t      written;

            remove(test_filename);

            file = nx_file_open(test_filename, "w");
            nx_assert(file != NULL, "nx_file_open failed to open file for writing");
            nx_assert(strcmp(file->filename, test_filename) == 0,
                      "nx_file_open did not set filename correctly");
            nx_assert(strcmp(file->mode, "w") == 0, "nx_file_open did not set mode correctly");

            written = fwrite(test_content, 1, strlen(test_content), file->file);
            nx_assert(written == strlen(test_content),
                      "fwrite did not write the correct number of bytes");

            nx_file_close(file);

            nx_assert(nx_file_exists(test_filename) == 1,
                      "nx_file_open/nx_file_close failed to create the file");

            remove(test_filename);
        }

        /* Test nx_file_write_all and nx_file_read_all */
        {
            const char *test_filename = "test_write_read.txt";
            const char *test_content  = "This is a test string for nx_file_write_all and "
                                        "nx_file_read_all.";
            int         write_result;
            char       *read_content;

            write_result = nx_file_write_all(test_filename, test_content);
            nx_assert(write_result == 0, "nx_file_write_all failed to write to the file");

            nx_assert(nx_file_exists(test_filename) == 1,
                      "nx_file_write_all did not create the file");

            read_content = nx_file_read_all(test_filename);
            nx_assert(read_content != NULL, "nx_file_read_all failed to read from the file");
            nx_assert(strcmp(read_content, test_content) == 0,
                      "nx_file_read_all read incorrect content");

            nx_free(read_content);

            remove(test_filename);
        }

        /* Test nx_file_exists */
        {
            const char *existing_file    = "existing_file.txt";
            const char *nonexistent_file = "nonexistent_file.txt";
            FILE       *file;

            file = fopen(existing_file, "w");
            nx_assert(file != NULL, "Failed to create existing_file.txt for "
                                    "testing nx_file_exists");
            fclose(file);

            nx_assert(nx_file_exists(existing_file) == 1,
                      "nx_file_exists failed to recognize existing file");
            nx_assert(nx_file_exists(nonexistent_file) == 0,
                      "nx_file_exists incorrectly reported a non-existent file "
                      "as existing");

            remove(existing_file);
        }

        /* Test nx_file_size */
        {
            const char *test_filename = "test_size.txt";
            const char *test_content  = "1234567890"; /* 10 bytes */
            int         write_result;
            long        size;
            const char *empty_filename;
            FILE       *empty_file;

            write_result = nx_file_write_all(test_filename, test_content);
            nx_assert(write_result == 0, "nx_file_write_all failed to write to "
                                         "the file for size testing");

            size = nx_file_size(test_filename);
            nx_assert(size == 10, "nx_file_size returned incorrect size");

            empty_filename = "empty_file.txt";
            empty_file     = fopen(empty_filename, "w");
            nx_assert(empty_file != NULL, "Failed to create empty_file.txt for size testing");
            fclose(empty_file);

            size = nx_file_size(empty_filename);
            nx_assert(size == 0, "nx_file_size did not return 0 for empty file");

            size = nx_file_size("no_such_file.txt");
            nx_assert(size == -1, "nx_file_size did not return -1 for non-existent file");

            remove(test_filename);
            remove(empty_filename);
        }

        /* Test nx_file_read_all with binary data */
        {
            const char   *test_filename = "test_binary.bin";
            unsigned char binary_data[] = {0x00, 0xFF, 0x7E, 0x81, 0x55};
            size_t        binary_size;
            FILE         *file;
            size_t        written;
            char         *read_data;

            binary_size = sizeof(binary_data);

            file = fopen(test_filename, "wb");
            nx_assert(file != NULL, "Failed to open test_binary.bin for writing");
            written = fwrite(binary_data, 1, binary_size, file);
            nx_assert(written == binary_size, "Failed to write binary data correctly");
            fclose(file);

            read_data = nx_file_read_all(test_filename);
            nx_assert(read_data != NULL, "nx_file_read_all failed to read binary data");
            nx_assert(memcmp(read_data, binary_data, binary_size) == 0,
                      "nx_file_read_all read incorrect binary data");
            nx_free(read_data);

            remove(test_filename);
        }

        /* Test nx_file_open with different modes */
        {
            const char *test_filename = "test_modes.txt";
            NXFile     *file_w;
            NXFile     *file_a;
            char       *content;

            file_w = nx_file_open(test_filename, "w");
            nx_assert(file_w != NULL, "nx_file_open failed with mode 'w'");
            fprintf(file_w->file, "Mode W Content\n");
            nx_file_close(file_w);

            file_a = nx_file_open(test_filename, "a");
            nx_assert(file_a != NULL, "nx_file_open failed with mode 'a'");
            fprintf(file_a->file, "Mode A Content\n");
            nx_file_close(file_a);

            content = nx_file_read_all(test_filename);
            nx_assert(content != NULL, "nx_file_read_all failed to read file with multiple writes");
            nx_assert(strstr(content, "Mode W Content") != NULL,
                      "File does not contain 'Mode W Content'");
            nx_assert(strstr(content, "Mode A Content") != NULL,
                      "File does not contain 'Mode A Content'");
            nx_free(content);

            remove(test_filename);
        }

        /* Test nx_file_open with invalid parameters */
        {
            NXFile *invalid_file;
            NXFile *nonexistent_file;

            invalid_file = nx_file_open("invalid_mode.txt", "invalid_mode");
            nx_assert(invalid_file == NULL, "nx_file_open did not fail with invalid mode");

            nonexistent_file = nx_file_open("nonexistent.txt", "r");
            nx_assert(nonexistent_file == NULL,
                      "nx_file_open did not fail when opening a non-existent "
                      "file for reading");
        }
    }

    nx_print_memory_leaks();
    printf("%sall tests passed.%s\n", COLOR_GREEN, COLOR_RESET);
    return 0;
}
