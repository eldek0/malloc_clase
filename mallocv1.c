#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 256

int main(void)
{
    char line[MAX_LINE];

    /*
     * fgets() reads one complete line from stdin.
     *
     * It stores at most sizeof(line) - 1 characters
     * and automatically appends '\0' at the end.
     *
     * The newline character '\n' is also copied
     * if there is enough space in the buffer.
     *
     * fgets() returns:
     *
     *   - a pointer to the buffer on success
     *   - NULL on EOF or read error
     */

    while (fgets(line, sizeof(line), stdin) != NULL) {

        /*
         * strtok() splits the string into tokens.
         *
         * Delimiters:
         *
         *   space
         *   tab
         *   newline
         *
         * strtok() modifies the original string:
         * delimiters are replaced with '\0'.
         *
         * First call:
         *
         *   strtok(line, ...)
         *
         * Subsequent calls:
         *
         *   strtok(NULL, ...)
         *
         * Returns:
         *
         *   - pointer to next token
         *   - NULL if no more tokens exist
         */

        char *cmd = strtok(line, " \t\n");

        /*
         * Empty line.
         *
         * Example:
         *
         *   user only pressed ENTER
         */

        if (cmd == NULL) {
            continue;
        }

        /*
         * m SIZE
         *
         * Allocate SIZE bytes.
         */

        if (strcmp(cmd, "m") == 0) {

            char *arg = strtok(NULL, " \t\n");

            /*
             * Missing argument.
             *
             * Example:
             *
             *   m
             */

            if (arg == NULL) {
                fprintf(stderr, "error: missing size\n");
                continue;
            }

            /*
             * Convert string to unsigned long.
             *
             * Example:
             *
             *   "100" -> 100
             */

            size_t size = strtoul(arg, NULL, 10);

            void *ptr = malloc(size);

            /*
             * malloc() failed.
             *
             * Real operating systems may fail because
             * of insufficient memory or resource limits.
             */

            if (ptr == NULL) {

                printf("0\n");
                fprintf(stderr, "error: not enough memory\n");

            } else {

                printf("%p\n", ptr);
            }
        }

        /*
         * f ADDRESS
         *
         * Free previously allocated memory.
         */

        else if (strcmp(cmd, "f") == 0) {

            char *arg = strtok(NULL, " \t\n");

            /*
             * Missing argument.
             *
             * Example:
             *
             *   f
             */

            if (arg == NULL) {
                fprintf(stderr, "error: missing pointer\n");
                continue;
            }

            void *ptr = NULL;

            /*
             * Convert textual hexadecimal address
             * into a real pointer value.
             *
             * Example:
             *
             *   "0x12345678"
             */

            sscanf(arg, "%p", &ptr);

            /*
             * Undefined behavior if ptr was not
             * returned by malloc().
             */
            free(ptr);
        }

        /*
         * q
         *
         * Quit program.
         */

        else if (strcmp(cmd, "q") == 0) {
            break;
        }

        else {
            fprintf(stderr, "error: unknown command\n");
        }
    }

    return 0;
}
