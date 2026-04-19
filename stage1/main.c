#include "syscall.h"

int c_main(void){
  sys_write(1, "Hello, world\n", 13);
  return 0;
}
