#ifndef MALLOC_H
#define MALLOC_H

#include <stddef.h>

/*
 * Memory layout:
 *   [block_header][user data........]
 *   ^             ^
 *   hdr           ptr returned to caller
 */
struct block_header {
	size_t size;
	int    free;
	struct block_header *next;
};

#define ALIGNMENT   16
#define ALIGN_UP(x) (((x) + (ALIGNMENT - 1)) & ~(size_t)(ALIGNMENT - 1))
#define HEADER_SIZE ALIGN_UP(sizeof(struct block_header))

void *my_malloc(size_t size);
void  my_free(void *ptr);

#endif
