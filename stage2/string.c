#include "string.h"

unsigned long strlen(const char *s)
{
	const char *p = s;
	while (*p)
		p++;
	return p - s;
}

void *memset(void *dst, int c, unsigned long n)
{
	unsigned char *p = dst;
	unsigned char val = (unsigned char)c;  // 截断为 unsigned char，避免符号扩展问题

	while (n--)
		*p++ = val;

	return dst;
}

void *memcpy(void *dst, const void *src, unsigned long n)
{
	unsigned char *d = dst;
	const unsigned char *s = src;

	while (n--)
		*d++ = *s++;

	return dst;
}
