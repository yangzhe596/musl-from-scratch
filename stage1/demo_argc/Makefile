CC      = gcc
CFLAGS  = -nostdlib -ffreestanding -fno-builtin -Wall -Wextra -Werror -g -no-pie
STAGE1  = ..

all: demo

demo: start.S main.c $(STAGE1)/syscall.c $(STAGE1)/syscall.h
	$(CC) $(CFLAGS) start.S main.c $(STAGE1)/syscall.c -o $@

run: demo
	./demo hello world foo=bar

clean:
	rm -f demo

.PHONY: all run clean
