# Day 03 - 手写 `_start`

## 今天的目标

- 写出最小的 `_start`
- 从汇编跳到你自己的 C 入口函数
- 把 C 函数返回值保留下来

## 预计投入

- 15 分钟：回顾 x86_64 SysV ABI
- 30 分钟：手写 `start.S`
- 15 分钟：反汇编验证跳转路径

## 今日任务

1. 在 `stage1/` 中新建：
   - `start.S`
   - `main.c`
2. 约定一个 C 入口，例如：

```c
int c_main(void);
```

3. 在 `_start` 中完成：
   - 调用 `c_main`
   - 拿到返回值
   - 暂时保留，下一天接 `exit`

## 今天只做最小实现

先不要处理：

- `argc`
- `argv`
- `envp`
- 栈对齐的更复杂场景

先让路径最短，后面再补理解。

## 建议编译命令

```bash
gcc -nostdlib -ffreestanding -fno-builtin -Wall -Wextra -Werror -g -no-pie \
    start.S main.c -o stage1/minimal
```

如果今天因为还没写 `exit` 导致链接或运行不完整，这是正常的。今天主要目标是把 `_start -> c_main` 这条路径搭出来。

## 验证标准

- 反汇编中能看到 `_start` 调用你的 C 函数
- 你能说清楚返回值当前放在哪个寄存器里

## musl 源码对照

- `crt/x86_64/crt1.s`

## 思考题

1. `_start` 调 C 函数时需要遵守哪套调用约定？
2. 为什么 syscall 又是另一套寄存器约定？
