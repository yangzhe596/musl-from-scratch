CC      = gcc
CFLAGS  = -nostdlib -ffreestanding -fno-builtin -Wall -Wextra -Werror -g -no-pie -I$(STAGE1)
STAGE1  = ../stage1

all: hello

hello: $(STAGE1)/start.S $(STAGE1)/syscall.c main.c printf.c printf.h
	$(CC) $(CFLAGS) $(STAGE1)/start.S main.c $(STAGE1)/syscall.c printf.c -o $@

run: hello
	./hello

disasm: hello
	objdump -d ./hello

clean:
	rm -f hello *.gch

.PHONY: all clean disasm run
