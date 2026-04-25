#include "printf.h"

static void test_put_char(void)
{
	put_char('A');
	put_char('B');
	put_char('\n');
}

static void test_put_str(void)
{
	put_str("hello");
	put_str(" ");
	put_str("world\n");
}

static void test_demo(void)
{
	demo("day10", "alpha", "beta", (const char *)0);
	demo("empty", (const char *)0);
}

int c_main(void)
{
	test_put_char();
	test_put_str();
	test_demo();
	return 0;
}
