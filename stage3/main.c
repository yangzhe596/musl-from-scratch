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

static void test_int_format(void)
{
	put_str("--- test_int_format ---\n");

	put_str("0 -> ");
	put_int(0);
	put_char('\n');

	put_str("123 -> ");
	put_int(123);
	put_char('\n');

	put_str("-1 -> ");
	put_int(-1);
	put_char('\n');

	put_str("-2147483648 -> ");
	put_int(-2147483647 - 1);
	put_char('\n');

	put_str("0x0 -> ");
	put_hex(0x0);
	put_char('\n');

	put_str("0xA -> ");
	put_hex(0xA);
	put_char('\n');

	put_str("0xDEAD -> ");
	put_hex(0xDEAD);
	put_char('\n');
}

static void test_demo(void)
{
	demo("day10", "alpha", "beta", (const char *)0);
	demo("empty", (const char *)0);
}

static void test_mini_printf(void)
{
	mini_printf("--- test_mini_printf ---\n");
	mini_printf("hello %s\n", "world");
	mini_printf("num=%d hex=%x\n", -1, 0xDEAD);
	mini_printf("null=%s\n", (const char *)0);
	mini_printf("100%% ok\n");
}

int c_main(void)
{
	test_put_char();
	test_put_str();
	test_demo();
	test_int_format();
	test_mini_printf();
	return 0;
}
