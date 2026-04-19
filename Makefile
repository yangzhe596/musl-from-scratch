CC      = gcc
CFLAGS  = -nostdlib -ffreestanding -fno-builtin -Wall -Wextra -Werror -g -no-pie

all: stage1/hello

stage1/hello: stage1/start.S stage1/main.c stage1/syscall.c stage1/syscall.h
	$(CC) $(CFLAGS) stage1/start.S stage1/main.c stage1/syscall.c -o $@

clean:
	rm -f stage1/hello stage1/minimal stage1/*.gch

.PHONY: all clean
