CC      = gcc
CFLAGS  = -nostdlib -ffreestanding -fno-builtin -Wall -Wextra -Werror -g -no-pie -I$(STAGE1) -I$(STAGE3)
STAGE1  = ../stage1
STAGE3  = ../stage3

all: hello

hello: $(STAGE1)/start.S $(STAGE1)/syscall.c $(STAGE3)/printf.c main.c malloc.c malloc.h
	$(CC) $(CFLAGS) $(STAGE1)/start.S main.c $(STAGE1)/syscall.c $(STAGE3)/printf.c malloc.c -o $@

run: hello
	./hello

disasm: hello
	objdump -d ./hello

clean:
	rm -f hello *.gch

.PHONY: all clean disasm run
