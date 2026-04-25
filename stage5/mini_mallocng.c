#include "mini_mallocng.h"
#include "syscall.h"

#define SYS_MMAP   9
#define SYS_MUNMAP 11

#define MAP_PRIVATE   0x02
#define MAP_ANONYMOUS 0x20
#define PROT_READ     0x1
#define PROT_WRITE    0x2
#define MMAP_FAILED   ((void *)-1)

static const size_t size_classes[] = {
	16, 32, 48, 64, 128, 256, 512, 1024
};
#define NUM_CLASSES (sizeof(size_classes) / sizeof(size_classes[0]))

static struct group *groups[NUM_CLASSES];

static void *sys_mmap(unsigned long length)
{
	return (void *)__syscall6(SYS_MMAP, 0, (long)length,
	                          PROT_READ | PROT_WRITE,
	                          MAP_PRIVATE | MAP_ANONYMOUS,
	                          -1, 0);
}

static void sys_munmap(void *addr, unsigned long length)
{
	__syscall2(SYS_MUNMAP, (long)addr, (long)length);
}

static int size_to_class(size_t size)
{
	for (int i = 0; i < (int)NUM_CLASSES; i++) {
		if (size <= size_classes[i])
			return i;
	}
	return -1;
}

static int bitmap_find_free(unsigned char *bitmap, int slot_count)
{
	for (int i = 0; i < slot_count; i++) {
		int byte = i / 8;
		int bit = i % 8;
		if (!(bitmap[byte] & (1 << bit)))
			return i;
	}
	return -1;
}

static void bitmap_set(unsigned char *bitmap, int index)
{
	bitmap[index / 8] |= (1 << (index % 8));
}

static void bitmap_clear(unsigned char *bitmap, int index)
{
	bitmap[index / 8] &= ~(1 << (index % 8));
}

static struct group *create_group(size_t size_class)
{
	unsigned long group_mem = GROUP_SIZE + size_class * SLOT_COUNT;
	void *mem = sys_mmap(group_mem);
	if (mem == MMAP_FAILED)
		return 0;

	struct group *g = (struct group *)mem;
	g->size_class = size_class;
	g->slot_count = SLOT_COUNT;
	g->free_count = SLOT_COUNT;
	g->next = 0;

	for (int i = 0; i < BITMAP_SIZE; i++)
		g->bitmap[i] = 0;

	return g;
}

static void *alloc_from_group(struct group *g)
{
	int slot = bitmap_find_free(g->bitmap, g->slot_count);
	if (slot < 0)
		return 0;

	bitmap_set(g->bitmap, slot);
	g->free_count--;

	char *base = (char *)g + GROUP_SIZE;
	return base + (size_t)slot * g->size_class;
}

void *mini_malloc(size_t size)
{
	if (size == 0)
		return 0;

	if (size > LARGE_THRESH) {
		unsigned long total = sizeof(size_t) + ALIGN_UP(size);
		void *mem = sys_mmap(total);
		if (mem == MMAP_FAILED)
			return 0;
		*(size_t *)mem = total;
		return (char *)mem + sizeof(size_t);
	}

	int class_idx = size_to_class(size);
	if (class_idx < 0)
		return 0;

	struct group *g = groups[class_idx];
	while (g) {
		if (g->free_count > 0)
			return alloc_from_group(g);
		g = g->next;
	}

	g = create_group(size_classes[class_idx]);
	if (!g)
		return 0;

	g->next = groups[class_idx];
	groups[class_idx] = g;

	return alloc_from_group(g);
}

void mini_free(void *ptr)
{
	if (!ptr)
		return;

	for (int i = 0; i < (int)NUM_CLASSES; i++) {
		struct group *g = groups[i];
		while (g) {
			char *base = (char *)g + GROUP_SIZE;
			char *end = base + (size_t)g->slot_count * g->size_class;
			if ((char *)ptr >= base && (char *)ptr < end) {
				int slot = (int)(((char *)ptr - base) / g->size_class);
				bitmap_clear(g->bitmap, slot);
				g->free_count++;
				return;
			}
			g = g->next;
		}
	}

	void *real = (char *)ptr - sizeof(size_t);
	sys_munmap(real, *(size_t *)real);
}
