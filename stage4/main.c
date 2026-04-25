#include "malloc.h"
#include "../stage3/printf.h"

static void test_basic_alloc(void)
{
	mini_printf("--- test_basic_alloc ---\n");

	void *p1 = my_malloc(16);
	mini_printf("alloc 16: %x\n", (unsigned int)(unsigned long)p1);

	void *p2 = my_malloc(32);
	mini_printf("alloc 32: %x\n", (unsigned int)(unsigned long)p2);

	void *p3 = my_malloc(8);
	mini_printf("alloc  8: %x\n", (unsigned int)(unsigned long)p3);

	my_free(p2);
	mini_printf("freed p2\n");

	void *p4 = my_malloc(24);
	mini_printf("alloc 24: %x (should reuse p2)\n", (unsigned int)(unsigned long)p4);

	my_free(p1);
	my_free(p3);
	my_free(p4);
	mini_printf("all freed\n");
}

static void test_alignment(void)
{
	mini_printf("--- test_alignment ---\n");

	for (int i = 1; i <= 32; i++) {
		void *p = my_malloc(i);
		unsigned long addr = (unsigned long)p;
		mini_printf("malloc(%d) -> %x (aligned=%d)\n",
		            i, (unsigned int)addr, (int)(addr % 16 == 0));
		my_free(p);
	}
}

static void test_zero_size(void)
{
	mini_printf("--- test_zero_size ---\n");

	void *p = my_malloc(0);
	mini_printf("malloc(0) -> %x (should be 0)\n", (unsigned int)(unsigned long)p);
}

static void test_write_read(void)
{
	mini_printf("--- test_write_read ---\n");

	int *arr = (int *)my_malloc(5 * sizeof(int));
	for (int i = 0; i < 5; i++)
		arr[i] = i * 100;

	for (int i = 0; i < 5; i++)
		mini_printf("arr[%d] = %d\n", i, arr[i]);

	my_free(arr);
}

int c_main(void)
{
	test_basic_alloc();
	test_alignment();
	test_zero_size();
	test_write_read();
	mini_printf("all tests passed\n");
	return 0;
}
