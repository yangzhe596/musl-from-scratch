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
	unsigned char val = (unsigned char)c;

	// Step 1: fill bytes until aligned to size_t boundary
	unsigned long align = ((8 - ((unsigned long)p & 7)) & 7);
	if (align > n)
		align = n;

	while (align--)
		*p++ = val;

	// Step 2: fill size_t blocks
	unsigned long word_val = val * 0x0101010101010101ULL;
	unsigned long *wp = (unsigned long *)p;
	unsigned long blocks = (n - align) / sizeof(unsigned long);

	while (blocks--)
		*wp++ = word_val;

	// Step 3: fill remaining bytes
	p = (unsigned char *)wp;
	unsigned long remain = (n - align) % sizeof(unsigned long);

	while (remain--)
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
