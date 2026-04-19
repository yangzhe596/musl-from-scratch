# Day 04 - 学习总结与标准答案

## 今日是否完成

Day 4 完成。

今天手写了 `sys_write` 和 `sys_exit` 两个 syscall 封装，实现了不依赖任何 libc 的 Hello World 程序。

## 实际代码

### `stage1/syscall.h`

```c
long sys_write(int fd, const void* buf, unsigned long count);
__attribute__((noreturn)) void sys_exit(int status);
```

### `stage1/syscall.c`

```c
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
```

### `stage1/main.c`

```c
#include "syscall.h"

int c_main(void){
  sys_write(1, "Hello, world\n", 13);
  return 0;
}
```

### `stage1/start.S`

```asm
.global _start

.text
_start:
  sub $8, %rsp
  call c_main
  mov %rax, %rdi
  call sys_exit
```

## 今日总结

今天最关键的进展是：第一次绕过 libc，直接用内联汇编发起 syscall。

x86_64 Linux syscall 约定与普通函数调用 ABI 的核心区别在于第 4 个参数：函数调用用 `rcx`，syscall 用 `r10`。原因是 `syscall` 指令执行时 CPU 硬件会占用 `rcx`（保存 rip）和 `r11`（保存 RFLAGS）。

GCC 内联汇编的约束方式（`"a"`/`"D"`/`"S"`/`"d"`）比显式 `mov` 更简洁高效——编译器直接把参数放到目标寄存器，避免冗余搬运指令。

## 踩过的坑

### 1. GCC 内联汇编语法不熟悉

- `%%` 双百分号转义寄存器名（因为 `%` 被操作数占位符占用）
- `\n` 分隔指令（GCC 把模板当一行字符串送给汇编器）
- 没有输出操作数时，三个 `:` 之间要留空位占位

### 2. `mov` 指令操作数大小不匹配

- `movq $1, %%rax` 报错：32 位立即数不能 `movq` 到 64 位寄存器
- `movq $1, %%eax` 报错：`movq` 是 64 位操作，`eax` 是 32 位寄存器
- 正确做法：`movl $1, %%eax`（写到 eax 自动零扩展到 rax）
- 最终用约束方式完全避开此问题

### 3. 编译时漏掉源文件

直接用 `cc` 编译时忘记加 `stage1/start.S`，链接时找不到 `_start` 符号。写 Makefile 解决。

## 标准答案

### 1. x86_64 Linux syscall 寄存器约定？

标准答案：

- `rax`：syscall 编号（write=1, exit=60）
- `rdi`、`rsi`、`rdx`、`r10`、`r8`、`r9`：前 6 个参数
- 返回值在 `rax`
- `rcx` 和 `r11` 被 `syscall` 指令破坏

与函数调用 ABI 的区别：第 4 个参数是 `r10` 而非 `rcx`。

### 2. 为什么 `sys_exit` 应该标成 `noreturn`？

标准答案：

标成 `noreturn` 告诉编译器此函数永远不会返回，编译器可以：
1. 删除函数调用之后的死代码（不需要的 `return` 语句）
2. 省略调用后的栈恢复和返回值处理代码
3. 消除"控制流到达非 void 函数末尾"的编译警告

配合 `__builtin_unreachable()` 使用，放在 `__asm__` 块之后。

### 3. clobber 列表为什么必须写？

标准答案：

- `"rcx"`：`syscall` 指令用 rcx 保存返回地址（rip），原值被覆盖
- `"r11"`：`syscall` 指令用 r11 保存 RFLAGS，原值被覆盖
- `"memory"`：`syscall` 可能读写内存（如 `write` 读取 buf 指向的内容），告诉编译器不要缓存内存值

漏掉 clobber，编译器可能把重要变量分配到 rcx/r11，syscall 一执行就被覆盖，产生难以调试的 bug。

### 4. GCC 内联汇编约束的含义？

标准答案：

| 约束 | 寄存器 | 名字来源 |
|------|--------|---------|
| `"a"` | rax | accumulator |
| `"D"` | rdi | destination index |
| `"S"` | rsi | source index |
| `"d"` | rdx | data |
| `"r"` | 任意通用寄存器 | register |

约束方式 vs 显式 mov：
- 约束方式：编译器直接把参数放到指定寄存器，零冗余
- 显式 mov：先用 `"r"` 约束放到任意寄存器，再 `mov` 搬到目标寄存器，多一条指令

## 今日最小知识闭环

- x86_64 syscall ABI：rax 放编号，rdi/rsi/rdx/r10/r8/r9 放参数
- syscall 与函数调用 ABI 的区别：第 4 个参数 r10 vs rcx，因为 rcx/r11 被硬件占用
- GCC 内联汇编三段式：输出操作数 / 输入操作数 / clobber 列表
- 约束字符串 `"a"`/`"D"`/`"S"`/`"d"` 对应特定寄存器
- `noreturn` + `__builtin_unreachable()` 标记永不返回的函数
- `sys_exit` 替代了 Day 03 的 `hlt` + `jmp .-1` 占位退出方式

## strace 验证

```
execve("./stage1/hello", ["./stage1/hello"], ...) = 0
write(1, "Hello, world\n", 13)          = 13
exit(0)                                 = ?
+++ exited with 0 +++
```

验证通过：
- 只看到 `write` 和 `exit` 两个 syscall ✓
- 输出正确 ✓
- 退出码为 0 ✓

## 进入 Day 05 前，你应该已经能回答

1. x86_64 syscall 约定中第 4 个参数为什么是 r10 而不是 rcx？
2. GCC 内联汇编的 clobber 列表漏掉 `rcx` 会怎样？
3. `noreturn` 对编译器优化有什么影响？
4. 约束方式 `"D"(fd)` 和显式 `mov %1, %%rdi` 哪个更好？为什么？
5. `_start` 里 `mov %rax, %rdi` 的作用是什么？
