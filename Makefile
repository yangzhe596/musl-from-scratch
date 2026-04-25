CC      = gcc
CFLAGS  = -nostdlib -ffreestanding -fno-builtin -Wall -Wextra -Werror -g -no-pie

all: stage1 stage2 stage3 stage4 stage5

stage1:
	$(MAKE) -C stage1

stage2:
	$(MAKE) -C stage2

stage3:
	$(MAKE) -C stage3

stage4:
	$(MAKE) -C stage4

stage5:
	$(MAKE) -C stage5

run-stage1:
	$(MAKE) -C stage1 run

run-stage2:
	$(MAKE) -C stage2 run

run-stage3:
	$(MAKE) -C stage3 run

run-stage4:
	$(MAKE) -C stage4 run

run-stage5:
	$(MAKE) -C stage5 run

clean:
	$(MAKE) -C stage1 clean
	$(MAKE) -C stage2 clean
	$(MAKE) -C stage3 clean
	$(MAKE) -C stage4 clean
	$(MAKE) -C stage5 clean

.PHONY: all stage1 stage2 stage3 stage4 stage5 run-stage1 run-stage2 run-stage3 run-stage4 run-stage5 clean
