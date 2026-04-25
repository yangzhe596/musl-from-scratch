#include "mini_mallocng.h"
#include "../stage3/printf.h"

static void test_small_alloc(void)
{
	mini_printf("--- test_small_alloc ---\n");

	void *p1 = mini_malloc(16);
	mini_printf("alloc 16: %x\n", (unsigned int)(unsigned long)p1);

	void *p2 = mini_malloc(16);
	mini_printf("alloc 16: %x\n", (unsigned int)(unsigned long)p2);

	void *p3 = mini_malloc(32);
	mini_printf("alloc 32: %x\n", (unsigned int)(unsigned long)p3);

	mini_free(p2);
	mini_printf("freed p2\n");

	void *p4 = mini_malloc(16);
	mini_printf("alloc 16: %x (should reuse p2's slot)\n", (unsigned int)(unsigned long)p4);

	mini_free(p1);
	mini_free(p3);
	mini_free(p4);
	mini_printf("all freed\n");
}

static void test_bitmap_correctness(void)
{
	mini_printf("--- test_bitmap_correctness ---\n");

	void *ptrs[8];
	for (int i = 0; i < 8; i++)
		ptrs[i] = mini_malloc(16);

	for (int i = 0; i < 8; i++)
		mini_free(ptrs[i]);

	for (int i = 0; i < 8; i++) {
		ptrs[i] = mini_malloc(16);
		if ((unsigned long)ptrs[i] != (unsigned long)ptrs[i]) {
			mini_printf("FAIL: slot not reused\n");
			return;
		}
	}

	mini_printf("bitmap reuse: OK\n");

	for (int i = 0; i < 8; i++)
		mini_free(ptrs[i]);
}

static void test_large_alloc(void)
{
	mini_printf("--- test_large_alloc ---\n");

	void *p = mini_malloc(4096);
	mini_printf("alloc 4096: %x\n", (unsigned int)(unsigned long)p);

	((char *)p)[0] = 'A';
	((char *)p)[4095] = 'Z';
	mini_printf("write OK: %c...%c\n", ((char *)p)[0], ((char *)p)[4095]);

	mini_free(p);
	mini_printf("freed large block\n");
}

static void test_write_read(void)
{
	mini_printf("--- test_write_read ---\n");

	int *arr = (int *)mini_malloc(5 * sizeof(int));
	for (int i = 0; i < 5; i++)
		arr[i] = i * 100;

	for (int i = 0; i < 5; i++)
		mini_printf("arr[%d] = %d\n", i, arr[i]);

	mini_free(arr);
}

int c_main(void)
{
	test_small_alloc();
	test_bitmap_correctness();
	test_large_alloc();
	test_write_read();
	mini_printf("all tests passed\n");
	return 0;
}
