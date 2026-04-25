# Stage 4 - 动态内存管理

## 实现功能

- `my_malloc(size)` — 从 mmap 获取内存，返回 16 字节对齐的指针
- `my_free(ptr)` — 标记块为空闲，合并相邻空闲块

## 内存布局

```text
[block_header][user data........]
^             ^
|             |
hdr           ptr returned to caller

struct block_header {
    size_t size;              // user data size
    int    free;              // 1 = free, 0 = in use
    struct block_header *next; // next in free list
};
```

## 设计决策

| 决策 | 选择 | 原因 |
|------|------|------|
| 后端 | `mmap` | 独立、语义直观、便于后续演进 |
| 对齐 | 16 字节 | x86_64 ABI 要求 |
| 搜索策略 | first-fit | 简单，够用 |
| 分裂 | 剩余空间 > header + 16 才分裂 | 避免碎片 |
| 合并 | 释放时线性扫描合并 | 简单，教学用 |

## 依赖

- `stage1/` 的 `start.S` 和 `syscall.c`
- `stage3/` 的 `printf.c`（用于测试输出）

## 编译与运行

```bash
make
make run
```
