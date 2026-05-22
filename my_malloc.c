/*
 * my_malloc.c
 *
 * Basic allocation functions for a simple memory allocator.
 */

#include "my_malloc.h"

#include <stdio.h>

/*
 * Searches the block list using the First Fit algorithm.
 *
 * First Fit means:
 *
 *   return the first free block that is large enough.
 */
BlockHeader *find_free_block_first_fit(size_t size) {
    BlockHeader *current = first_block; /* Start at the beginning of the block list. */

    while (current != NULL) { /* Traverse all blocks. */

        if (current->is_free && current->size >= size) {
            return current; /* First suitable free block found. */
        }

        current = current->next; /* Move to next block. */
    }

    return NULL; /* No suitable block was found. */
}

/*
 * Splits a free block if it is larger than needed.
 *
 * Example:
 *
 *   [ free block: 1000 bytes ]
 *
 * malloc(100)
 *
 *   [ used block: 100 bytes ][ free block: remaining bytes ]
 */
void split_block(BlockHeader *block, size_t size) {
    BlockHeader *new_block;
    char *new_block_address;
    size_t remaining_size;

    /*
     * If the block is not large enough to contain:
     *
     *   requested user space
     *   + one new BlockHeader
     *   + at least one usable byte
     *
     * then we do not split it.
     */
    if (block->size <= size + sizeof(BlockHeader)) {
        return;
    }

    remaining_size = block->size - size - sizeof(BlockHeader);

    /*
     * block + 1 points just after the current header.
     * That is the beginning of the user data area.
     *
     * Adding size bytes gives the address where the new header starts.
     */
    new_block_address = (char *)(block + 1) + size;

    new_block = (BlockHeader *)new_block_address;

    new_block->size = remaining_size; /* The new block contains the remaining free space. */
    new_block->is_free = 1;           /* The new block is free. */
    new_block->next = block->next;    /* The new block points to the old next block. */

    block->size = size;      /* The original block now has the requested size. */
    block->next = new_block; /* The original block is followed by the new block. */
}

/*
 * Allocates memory from the custom allocator.
 *
 * This function:
 *
 *   1. validates the requested size
 *   2. finds a suitable free block
 *   3. splits the block if possible
 *   4. marks the block as used
 *   5. returns a pointer to the usable memory area
 */
void *my_malloc(size_t size) {
    BlockHeader *block;

    if (size == 0) {
        return NULL; /* malloc(0) is ignored in this simplified allocator. */
    }

    block = find_free_block_first_fit(size); /* For now, we always use First Fit. */

    if (block == NULL) {
        fprintf(stderr, "error: not enough memory\n");
        return NULL;
    }

    split_block(block, size); /* Split only if there is enough remaining space. */

    block->is_free = 0; /* Mark the selected block as allocated. */

    return (void *)(block + 1); /* Return the address immediately after the header. */
}

void my_malloc_print_state(void) {
    BlockHeader *current = first_block;
    while (current != NULL) {
        printf("%p %s size=%zu\n",
            (void *)current,
            current->is_free ? "FREE" : "USED",
            current->size);
        current = current->next;
    }
}
void my_free(void *ptr) {
    if (ptr == NULL) return;

    BlockHeader *block = (BlockHeader *)ptr - 1;
    block->is_free = 1;
}