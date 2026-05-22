/*
 * allocator_test.c
 */

#include "my_malloc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 256

int main(int argc, char *argv[]) {
    char line[MAX_LINE];

    if (argc != 2) {
        fprintf(stderr, "usage: %s ARENA_SIZE\n", argv[0]);
        return 1;
    }

    size_t arena_size = strtoul(argv[1], NULL, 10);

    if (my_malloc_init(arena_size) != 0) {
        fprintf(stderr, "allocator initialization failed\n");
        return 1;
    }

    printf("Arena size: %zu bytes\n", arena_size);

    while (fgets(line, sizeof(line), stdin) != NULL) {

        char *cmd = strtok(line, " \t\n");

        if (cmd == NULL) {
            continue;
        }

        /*
         * m SIZE
         */

        if (strcmp(cmd, "m") == 0) {

            char *arg = strtok(NULL, " \t\n");

            if (arg == NULL) {
                fprintf(stderr, "missing size\n");
                continue;
            }

            size_t size = strtoul(arg, NULL, 10);

            void *ptr = my_malloc(size);

            /*
             * Print returned address.
             * NULL is printed as 0.
             */

            printf("%p\n", ptr);
        }

        /*
         * q
         */

        else if (strcmp(cmd, "q") == 0) {

            break;
        }

        else {

            fprintf(stderr, "unknown command\n");
        }
    }

    return 0;
}
