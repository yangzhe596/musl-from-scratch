#include "syscall.h"

long sys_write(int fd, const void* buf, unsigned long count){
  long ret = __syscall3(1, fd, (long)buf, count);
  return ret;
}

__attribute__((noreturn)) void sys_exit(int status){
  __syscall1(60, status);
  __builtin_unreachable();
}