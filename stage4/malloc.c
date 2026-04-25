#include "malloc.h"
#include "syscall.h"

/* x86_64 Linux syscall numbers */
#define SYS_MMAP   9
#define SYS_MUNMAP 11

#define MAP_PRIVATE   0x02
#define MAP_ANONYMOUS 0x20
#define PROT_READ     0x1
#define PROT_WRITE    0x2

#define MMAP_FAILED   ((void *)-1)

static void *sys_mmap(void *addr, unsigned long length,
                       int prot, int flags,
                       int fd, unsigned long offset)
{
	return (void *)__syscall6(SYS_MMAP, (long)addr, (long)length,
	                          prot, flags, fd, (long)offset);
}

static __attribute__((unused)) long sys_munmap(void *addr, unsigned long length)
{
	return __syscall2(SYS_MUNMAP, (long)addr, (long)length);
}

/* Head of the free list */
static struct block_header *free_list = (void *)0;

/*
 * Find a free block that can hold `size` bytes.
 * Simple first-fit search.
 */
static struct block_header *find_free(size_t size)
{
	struct block_header *cur = free_list;

	while (cur) {
		if (cur->free && cur->size >= size)
			return cur;
		cur = cur->next;
	}

	return (void *)0;
}

/*
 * Request a new block from the OS via mmap.
 * Returns pointer to the new block header, or NULL on failure.
 */
static struct block_header *request_block(size_t size)
{
	unsigned long total = HEADER_SIZE + size;
	void *p = sys_mmap((void *)0, total,
	                   PROT_READ | PROT_WRITE,
	                   MAP_PRIVATE | MAP_ANONYMOUS,
	                   -1, 0);

	if (p == MMAP_FAILED)
		return (void *)0;

	struct block_header *hdr = (struct block_header *)p;
	hdr->size = size;
	hdr->free = 0;
	hdr->next = (void *)0;
	return hdr;
}

/*
 * Split a free block if it's significantly larger than requested.
 * This avoids wasting memory on oversized allocations.
 */
static void split_block(struct block_header *hdr, size_t size)
{
	/* Only split if there's room for header + at least 16 bytes */
	if (hdr->size < size + HEADER_SIZE + ALIGNMENT)
		return;

	/* New block starts right after current header + requested size */
	struct block_header *new_hdr =
		(struct block_header *)((char *)hdr + HEADER_SIZE + size);

	new_hdr->size = hdr->size - size - HEADER_SIZE;
	new_hdr->free = 1;
	new_hdr->next = hdr->next;

	hdr->size = size;
	hdr->next = new_hdr;
}

void *my_malloc(size_t size)
{
	if (size == 0)
		return (void *)0;

	/* Align size up to 16 bytes */
	size = ALIGN_UP(size);

	/* Try to find an existing free block */
	struct block_header *hdr = find_free(size);

	if (hdr) {
		split_block(hdr, size);
		hdr->free = 0;
	} else {
		/* No suitable block found, request from OS */
		hdr = request_block(size);
		if (!hdr)
			return (void *)0;

		/* Prepend to free list */
		hdr->next = free_list;
		free_list = hdr;
		hdr->free = 0;
	}

	/* Return pointer to user data, not to header */
	return (char *)hdr + HEADER_SIZE;
}

/*
 * Merge adjacent free blocks (coalescing).
 * Simple linear scan from the head of the free list.
 */
static void merge_free(void)
{
	struct block_header *cur = free_list;

	while (cur && cur->next) {
		/* Check if cur and cur->next are adjacent in memory */
		char *cur_end = (char *)cur + HEADER_SIZE + cur->size;
		if (cur_end == (char *)cur->next && cur->next->free) {
			/* Merge cur->next into cur */
			cur->size += HEADER_SIZE + cur->next->size;
			cur->next = cur->next->next;
			/* Don't advance - check if we can merge more */
		} else {
			cur = cur->next;
		}
	}
}

void my_free(void *ptr)
{
	if (!ptr)
		return;

	/* Get block header from user pointer */
	struct block_header *hdr =
		(struct block_header *)((char *)ptr - HEADER_SIZE);

	hdr->free = 1;

	/* Try to merge adjacent free blocks */
	merge_free();
}
