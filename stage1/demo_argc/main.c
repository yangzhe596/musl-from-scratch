#include "../syscall.h"

static void put_str(const char *s)
{
	if (!s)
		return;
	const char *p = s;
	while (*p) p++;
	sys_write(1, s, (unsigned long)(p - s));
}

static void put_char(char c)
{
	sys_write(1, &c, 1);
}

static void put_int(long x)
{
	char buf[20];
	int i = 0;
	unsigned long u;

	if (x < 0) {
		put_char('-');
		u = ~(unsigned long)x + 1;
	} else {
		u = (unsigned long)x;
	}

	do {
		buf[i++] = '0' + (char)(u % 10);
		u /= 10;
	} while (u);

	while (--i >= 0)
		put_char(buf[i]);
}

int c_main(unsigned long *sp)
{
	int argc = (int)sp[0];
	char **argv = (char **)(sp + 1);
	char **envp = argv + argc + 1;

	put_str("argc = ");
	put_int(argc);
	put_char('\n');

	put_str("\n--- argv ---\n");
	for (int i = 0; i < argc; i++) {
		put_str("  argv[");
		put_int(i);
		put_str("] = \"");
		put_str(argv[i]);
		put_str("\"\n");
	}

	put_str("\n--- envp ---\n");
	for (int i = 0; envp[i]; i++) {
		put_str("  envp[");
		put_int(i);
		put_str("] = \"");
		put_str(envp[i]);
		put_str("\"\n");
	}

	return 0;
}
