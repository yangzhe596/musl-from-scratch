.global _start

.text
_start:
  mov %rsp, %rdi
  and $-16, %rsp
  call c_main
  mov %rax, %rdi
  mov $60, %rax
  syscall
