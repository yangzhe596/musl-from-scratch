#ifndef PRINTF_H
#define PRINTF_H

void put_char(char c);
void put_str(const char *s);
void put_int(int x);
void put_hex(unsigned int x);
int demo(const char *tag, ...);
int mini_printf(const char *fmt, ...);

#endif
