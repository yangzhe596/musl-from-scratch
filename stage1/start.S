.global _start

.text
_start:
  sub $8, %rsp
  call c_main
  mov %rax, %rdi
  call sys_exit
