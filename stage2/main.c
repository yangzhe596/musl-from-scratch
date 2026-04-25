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

static void print_hex(unsigned char c)
{
	char hex[] = "0123456789abcdef";
	sys_write(1, &hex[c >> 4], 1);
	sys_write(1, &hex[c & 0x0f], 1);
}

static void print_memory(const void *addr, unsigned long n)
{
	const unsigned char *p = addr;
	for (unsigned long i = 0; i < n; i++) {
		if (i > 0)
			sys_write(1, " ", 1);
		print_hex(p[i]);
	}
	sys_write(1, "\n", 1);
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

static void test_memset(void)
{
	unsigned char buf[16];

	sys_write(1, "--- test_memset ---\n", 20);

	// Test 1: n == 0
	memset(buf, 0xAA, 0);
	sys_write(1, "n=0: ", 5);
	print_memory(buf, 4);

	// Test 2: fill with 0x00
	memset(buf, 0x00, 8);
	sys_write(1, "fill 0x00: ", 11);
	print_memory(buf, 8);

	// Test 3: fill with 0xFF (test sign extension avoidance)
	memset(buf, 0xFF, 8);
	sys_write(1, "fill 0xFF: ", 11);
	print_memory(buf, 8);

	// Test 4: fill with custom value
	memset(buf, 0x42, 6);
	sys_write(1, "fill 0x42: ", 11);
	print_memory(buf, 6);

	// Test 5: aligned address (buf is likely aligned)
	memset(buf, 0x55, 8);
	sys_write(1, "aligned 8: ", 11);
	print_memory(buf, 8);

	// Test 6: unaligned address
	memset(buf + 1, 0x77, 7);
	sys_write(1, "unaligned 7: ", 13);
	print_memory(buf, 8);

	// Test 7: small n (less than size_t)
	memset(buf, 0x99, 3);
	sys_write(1, "small n=3: ", 11);
	print_memory(buf, 4);
}

static void test_memcpy(void)
{
	unsigned char src[] = {0x01, 0x02, 0x03, 0x04, 0x05};
	unsigned char dst[8];

	sys_write(1, "--- test_memcpy ---\n", 20);

	// Test 1: n == 0
	memcpy(dst, src, 0);
	sys_write(1, "n=0: ", 5);
	print_memory(dst, 4);

	// Test 2: normal copy
	memset(dst, 0, sizeof(dst));
	memcpy(dst, src, 5);
	sys_write(1, "copy 5 bytes: ", 14);
	print_memory(dst, 5);

	// Test 3: copy to different offset
	memset(dst, 0, sizeof(dst));
	memcpy(dst + 2, src, 3);
	sys_write(1, "copy to offset 2: ", 18);
	print_memory(dst, 6);
}

int c_main(void)
{
	test_strlen("");
	test_strlen("hello");
	test_strlen("a");
	test_strlen("1234567890");
	test_memset();
	test_memcpy();
	return 0;
}
