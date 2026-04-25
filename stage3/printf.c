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
