#ifndef MINI_MALLOCNG_H
#define MINI_MALLOCNG_H

#include <stddef.h>

#define SLOT_COUNT    256
#define BITMAP_SIZE   (SLOT_COUNT / 8)
#define LARGE_THRESH  1024

#define ALIGNMENT     16
#define ALIGN_UP(x)   (((x) + (ALIGNMENT - 1)) & ~(size_t)(ALIGNMENT - 1))

struct group {
	size_t size_class;
	int    slot_count;
	int    free_count;
	unsigned char bitmap[BITMAP_SIZE];
	struct group *next;
};

#define GROUP_SIZE (ALIGN_UP(sizeof(struct group)))

void *mini_malloc(size_t size);
void  mini_free(void *ptr);

#endif
