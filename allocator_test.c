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

    printf("Type 'help' for avaliable commands");
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

            printf("%p", ptr);
            /* additionally print availability/status */
            if (ptr == NULL) {
                printf(" -> NULL\n");
            } else {
                int st = my_malloc_check_ptr(ptr);
                if (st == 1) printf(" -> ALLOCATED\n");
                else if (st == 2) printf(" -> FREE\n");
                else printf(" -> NOT IN ARENA\n");
            }
        }

        /*
         * h | help
         *
         * Print available commands.
         */
        else if (strcmp(cmd, "h") == 0 || strcmp(cmd, "help") == 0) {
            printf("Available commands:\n");
            printf("  m SIZE    - allocate SIZE bytes using my_malloc and print address (NULL -> 0)\n");
            printf("  f ADDRESS - free previously allocated pointer (use address returned by m)\n");
            printf("  p         - print allocator internal state (blocks list)\n");
            printf("  c ADDRESS - check if ADDRESS is inside arena and if it's free or used\n");
            printf("  h|help    - show this help message\n");
            printf("  q         - quit\n");
        }

        else if (strcmp(cmd, "p") == 0) {
            my_malloc_print_state();
        }

        else if (strcmp(cmd, "c") == 0) {
            char *arg = strtok(NULL, " \t\n");
            if (arg == NULL) {
                fprintf(stderr, "missing address\n");
                continue;
            }
            void *ptr = NULL;
            sscanf(arg, "%p", &ptr);
            int res = my_malloc_check_ptr(ptr);
            if (res == 0) printf("%p -> NOT IN ARENA\n", ptr);
            else if (res == 1) printf("%p -> ALLOCATED\n", ptr);
            else if (res == 2) printf("%p -> FREE\n", ptr);
        }

        else if (strcmp(cmd, "f") == 0) {
            char *arg = strtok(NULL, " \t\n");
            if (arg == NULL) {
                fprintf(stderr, "missing address\n");
                continue;
            }
            void *ptr = NULL;
            sscanf(arg, "%p", &ptr);
            my_free(ptr);
        }
        else if (strcmp(cmd, "q") == 0) {

            break;
        }
        

        else {

            fprintf(stderr, "unknown command\n");
        }
    }

    return 0;
}
