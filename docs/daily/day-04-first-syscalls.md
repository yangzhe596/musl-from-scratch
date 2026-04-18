# Day 04 - 第一个 syscall：`write` 与 `exit`

## 今天的目标

- 掌握 x86_64 Linux syscall 调用约定
- 实现 `write` 和 `exit`
- 打印出不依赖 glibc 的 Hello World

## 预计投入

- 15 分钟：理解 syscall 寄存器约定
- 30 分钟：写内联汇编封装
- 15 分钟：运行和 `strace` 验证

## 今日任务

1. 在 `stage1/` 中增加：
   - `syscall.h`
   - `syscall.c` 或纯头文件实现
2. 手写：
   - `long sys_write(int fd, const void *buf, unsigned long count);`
   - `__attribute__((noreturn)) void sys_exit(int status);`
3. 修改 `c_main`：
   - 使用 `sys_write` 打印 `"Hello, world\n"`
   - 返回一个状态码
4. 修改 `_start`：
   - 调用 `c_main`
   - 把返回值传给 `sys_exit`

## 关键知识

x86_64 Linux syscall 约定：

- `rax`：syscall 编号
- `rdi`, `rsi`, `rdx`, `r10`, `r8`, `r9`：前 6 个参数
- 返回值放在 `rax`
- `rcx`, `r11` 会被破坏

## 建议编译命令

```bash
gcc -nostdlib -ffreestanding -fno-builtin -Wall -Wextra -Werror -g -no-pie \
    stage1/start.S stage1/main.c stage1/syscall.c -o stage1/hello
```

## 验证命令

```bash
./stage1/hello
strace ./stage1/hello
objdump -d ./stage1/hello | less
```

## 验证标准

- 程序输出 `Hello, world`
- `strace` 中只看到你预期的 `write` 和 `exit`
- 你能解释内联汇编里的 clobber 为什么必须写

## musl 源码对照

- `arch/x86_64/syscall_arch.h`
- `src/unistd/write.c`
- `src/exit/_Exit.c`

## 思考题

1. 为什么 `sys_exit` 应该标成 `noreturn`？
2. 如果 clobber 漏掉 `rcx` 或 `r11`，编译器为什么可能生成错误代码？
