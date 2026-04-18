.global _start

.text
_start:
  sub $8, %rsp
  call c_main
  hlt
  jmp .-1
