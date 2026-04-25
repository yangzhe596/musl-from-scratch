#include "syscall.h"
#include "printf.h"

#include <stdarg.h>

void put_char(char c)
{
	sys_write(1, &c, 1);
}

void put_str(const char *s)
{
	if (!s)
		return;

	while (*s)
		put_char(*s++);
}

int demo(const char *tag, ...)
{
	va_list ap;
	const char *s;
	int count = 0;

	va_start(ap, tag);
	put_str("[demo] tag=");
	put_str(tag);

	while ((s = va_arg(ap, const char *)) != 0) {
		put_str(" | ");
		put_str(s);
		count++;
	}

	va_end(ap);
	put_char('\n');
	return count;
}

static char *utoa10(unsigned int x, char *end)
{
	char *p = end;

	if (x == 0) {
		*--p = '0';
		return p;
	}

	while (x) {
		unsigned int d = x % 10;
		*--p = (char)('0' + d);
		x /= 10;
	}

	return p;
}

static char *utoa16(unsigned int x, char *end)
{
	static const char hex[] = "0123456789abcdef";
	char *p = end;

	if (x == 0) {
		*--p = '0';
		return p;
	}

	while (x) {
		unsigned int d = x & 0x0f;
		*--p = hex[d];
		x >>= 4;
	}

	return p;
}

static char *itoa10(int x, char *end)
{
	if (x < 0) {
		unsigned int u = ~(unsigned int)x + 1u;
		char *p = utoa10(u, end);
		*--p = '-';
		return p;
	}

	return utoa10((unsigned int)x, end);
}

void put_int(int x)
{
	char buf[16];
	char *p = itoa10(x, buf + sizeof(buf));

	while (p < buf + sizeof(buf))
		put_char(*p++);
}

void put_hex(unsigned int x)
{
	char buf[16];
	char *p = utoa16(x, buf + sizeof(buf));

	while (p < buf + sizeof(buf))
		put_char(*p++);
}

int mini_printf(const char *fmt, ...)
{
	va_list ap;
	int written = 0;

	va_start(ap, fmt);

	while (*fmt) {
		if (*fmt != '%') {
			put_char(*fmt++);
			written++;
			continue;
		}

		fmt++;

		switch (*fmt) {
		case '%':
			put_char('%');
			written++;
			break;
		case 's': {
			const char *s = va_arg(ap, const char *);

			if (!s)
				s = "(null)";

			while (*s) {
				put_char(*s++);
				written++;
			}
			break;
		}
		case 'd':
			put_int(va_arg(ap, int));
			break;
		case 'x':
			put_hex(va_arg(ap, unsigned int));
			break;
		default:
			put_char('%');
			put_char(*fmt);
			written += 2;
			break;
		}

		fmt++;
	}

	va_end(ap);
	return written;
}
