# Stage 1 - 无 libc 启动

## 支持功能

- 手写 `_start` 入口，不依赖 glibc CRT
- 内联汇编封装 `sys_write` 和 `sys_exit` syscall
- 打印 Hello World 并正常退出

## 编译与运行

```bash
make          # 编译
make run      # 编译并运行
make disasm   # 编译并反汇编
make clean    # 清理
```

## 文件说明

| 文件 | 职责 |
|------|------|
| `start.S` | `_start` 入口，调用 `c_main` 后将返回值传给 `sys_exit` |
| `main.c` | C 入口函数 `c_main`，调用 `sys_write` 输出 |
| `syscall.h` | syscall 函数声明 |
| `syscall.c` | `sys_write` / `sys_exit` 内联汇编实现 |

## 踩过的坑

### 1. GCC 内联汇编语法不熟悉

- `%%` 双百分号转义寄存器名（因为 `%` 被操作数占位符占用）
- `\n` 分隔指令（GCC 把模板当一行字符串送给汇编器）
- 没有输出操作数时，三个 `:` 之间要留空位占位

## 验证

```bash
strace ./hello
# 应只看到 write(1, "Hello, world\n", 13) 和 exit(0)
```
