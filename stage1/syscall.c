
long sys_write(int fd, const void* buf, unsigned long count){
  long ret;
  __asm__ __volatile(
    "syscall"
    : "=a"(ret)
    : "a"(1), "D"(fd), "S"(buf), "d"(count)
    : "rcx", "r11", "memory"
  );
  return ret;
}

__attribute__((noreturn)) void sys_exit(int status){
  __asm__ __volatile(
    "syscall"
    :
    : "a"(60), "D"(status)
    : "rcx", "r11", "memory"
  );
  __builtin_unreachable();
}