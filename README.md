# musl-from-scratch

面向 x86_64 Linux 的手写 libc 学习项目。从 ELF 入口到分槽分配器，22 天完整走完。

## 实现内容

| 阶段 | 天数 | 核心内容 |
|------|------|----------|
| Stage 1 | Day 01-05 | `_start`、syscall 封装、`write`/`exit` |
| Stage 2 | Day 06-09 | `strlen`、`memset`、`memcpy`、对齐优化 |
| Stage 3 | Day 10-13 | `va_list`、整数格式化、最小 `printf` |
| Stage 4 | Day 14-18 | `mmap`、free list `malloc`/`free`、合并 |
| Stage 5 | Day 19-22 | mallocng 设计、size class、group、bitmap |

## 目录结构

```text
.
├── README.md
├── Makefile
├── docs/
│   ├── daily/          # Day 01-22 每日任务文档
│   └── answers/        # Day 01-22 总结与标准答案
├── stage1/             # _start + syscall
├── stage2/             # 字符串/内存基础
├── stage3/             # 最小 printf
├── stage4/             # free list 分配器
└── stage5/             # 分槽分配器 (mini_mallocng)
```

## 编译与运行

```bash
# 编译所有阶段
make all

# 编译并运行单个阶段
make stage1    # _start + syscall
make stage2    # strlen/memset/memcpy
make stage3    # mini_printf
make stage4    # free list malloc
make stage5    # mini_mallocng

# 清理所有构建产物
make clean
```

## 各阶段说明

### Stage 1 - 无 libc 启动

手写 `_start` 入口，不依赖 glibc CRT。用内联汇编封装 `sys_write` 和 `sys_exit`。

### Stage 2 - 字符串/内存基础

实现 `strlen`、`memset`、`memcpy`。包含三步走对齐优化和字长批量处理。

### Stage 3 - 格式化输出

实现最小 `printf`，支持 `%s`、`%d`、`%x`、`%%`。从 `va_list` 实验到格式解析状态机。

### Stage 4 - 动态内存管理

用 `mmap` 实现 free list 分配器。支持 first-fit 搜索、块分裂、相邻合并。

### Stage 5 - 分槽分配器

借鉴 musl mallocng 设计，用 size class + group + bitmap 实现 O(1) 小对象分配。

## 编译参数

```bash
gcc -nostdlib -ffreestanding -fno-builtin -Wall -Wextra -Werror -g -no-pie
```

- `-nostdlib`：不链接标准库
- `-ffreestanding`：独立环境，不假设标准库存在
- `-fno-builtin`：禁止编译器内建函数替换
- `-no-pie`：固定地址，简化调试

## 验证工具

```bash
readelf -h ./hello    # 查看 ELF 头
objdump -d ./hello    # 反汇编
strace ./hello        # 跟踪系统调用
```

## 学习资料

- `docs/daily/` — 每日任务文档，按 Day 01-22 组织
- `docs/answers/` — 每日总结与标准答案

## 参考资料

- [musl 源码](https://git.musl-libc.org/cgit/musl)
- System V AMD64 ABI
- Linux x86_64 syscall ABI
- *Computer Systems: A Programmer's Perspective*
- *Operating Systems: Three Easy Pieces*
