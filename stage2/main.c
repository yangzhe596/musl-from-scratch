#include "syscall.h"
#include "string.h"

static void print_ulong(unsigned long val)
{
	char buf[20];
	int i = 0;

	do {
		buf[i++] = val % 10 + '0';
		val /= 10;
	} while (val);

	while (--i >= 0)
		sys_write(1, &buf[i], 1);
}

static void test_strlen(const char *s)
{
	unsigned long len = strlen(s);

	sys_write(1, "\"", 1);
	if (len > 0)
		sys_write(1, s, len);
	sys_write(1, "\" -> ", 5);
	print_ulong(len);
	sys_write(1, "\n", 1);
}

int c_main(void)
{
	test_strlen("");
	test_strlen("hello");
	test_strlen("a");
	test_strlen("1234567890");
	return 0;
}
