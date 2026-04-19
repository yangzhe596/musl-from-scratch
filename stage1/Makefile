CC      = gcc
CFLAGS  = -nostdlib -ffreestanding -fno-builtin -Wall -Wextra -Werror -g -no-pie

all: hello

hello: start.S main.c syscall.c syscall.h
	$(CC) $(CFLAGS) start.S main.c syscall.c -o $@

run: hello
	./hello

disasm: hello
	objdump -d ./hello

clean:
	rm -f hello *.gch

.PHONY: all clean disasm run
