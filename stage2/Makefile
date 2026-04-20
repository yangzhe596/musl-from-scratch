CC      = gcc
CFLAGS  = -nostdlib -ffreestanding -fno-builtin -Wall -Wextra -Werror -g -no-pie -I$(STAGE1)
STAGE1  = ../stage1

all: test_strlen

test_strlen: $(STAGE1)/start.S $(STAGE1)/syscall.c main.c string.c string.h
	$(CC) $(CFLAGS) $(STAGE1)/start.S main.c $(STAGE1)/syscall.c string.c -o $@

run: test_strlen
	./test_strlen

disasm: test_strlen
	objdump -d ./test_strlen

clean:
	rm -f test_strlen *.gch

.PHONY: all clean disasm run
