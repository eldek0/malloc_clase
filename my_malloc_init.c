#include "my_malloc.h"

#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>

void *arena_start = NULL;
BlockHeader *first_block = NULL;

int my_malloc_init(size_t arena_size) {
    arena_start =
        mmap(NULL, arena_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (arena_start == MAP_FAILED) {
        perror("mmap");
        arena_start = NULL;
        first_block = NULL;
        return -1;
    }

    /*
     * The first bytes of the arena are used to store
     * the header of the initial free block.
     *
     * The usable space starts immediately after this header.

         arena_start
            ↓
         +----------------------+--------------------------------+
         | BlockHeader          | usable free memory             |
         | size = ARENA_SIZE... |                                |
         | is_free = 1          |                                |
         | next = NULL          |                                |
         +----------------------+--------------------------------+
     */

    first_block = (BlockHeader *)arena_start;

    first_block->size = arena_size - sizeof(BlockHeader);
    first_block->is_free = 1;
    first_block->next = NULL;

    return 0;
}
