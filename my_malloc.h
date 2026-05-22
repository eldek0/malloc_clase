/*
 * my_malloc.h
 *
 * Definitions and prototypes for the custom memory allocator.
 */

#ifndef MY_MALLOC_H
#define MY_MALLOC_H

#include <stddef.h>

/*
 * A block is stored inside the arena itself.
 *
 * Each block has:
 *
 *   - size: usable space available after this header
 *   - is_free: 1 if the block is free, 0 if it is allocated
 *   - next: pointer to the next block in the arena
 */
typedef struct block_header {
    size_t size;                // usable space available after this header
    int is_free;                // 1 if the block is free, 0 if it is allocated
    struct block_header *next;  // pointer to the next block in the arena
} BlockHeader;

/*
 * Initializes the allocator.
 *
 * It requests one large contiguous memory area using mmap().
 * Initially, the whole arena is represented as one single free block.
 */

extern void *arena_start;
extern BlockHeader *first_block;

/*
 * Functions already implemented
 */

int my_malloc_init(size_t arena_size);
BlockHeader *find_free_block_first_fit(size_t size);
void split_block(BlockHeader *block, size_t size);
void *my_malloc(size_t size);

/*
 * Future functions to be implemented by students.
 */

void my_free(void *ptr);
void my_malloc_print_state(void);
/*
 * Check a pointer: returns
 *   0 -> pointer not in arena
 *   1 -> pointer in arena and allocated
 *   2 -> pointer in arena and free
 */
int my_malloc_check_ptr(void *ptr);

#endif
